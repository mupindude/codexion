/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 13:39:03 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 07:17:33 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	find_best_eligible_request(t_data *data, long now);
static void	log_one_dongle(t_coder *coder);
static void	log_dongle_acquisition(t_coder *coder);

void	*scheduler_routine(void *arg)
{
	t_data		*data;
	t_request	*request;
	int			best;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->scheduler_mutex);
		while (data->waiters->size == 0 && data->stop == 0)
			pthread_cond_wait(&data->scheduler_cond, &data->scheduler_mutex);
		if (data->stop)
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			break ;
		}
		best = find_best_eligible_request(data, get_time_ms());
		if (best != -1)
		{
			request = data->waiters->entries[best];
			if (try_reserve_dongles(request, get_time_ms()))
			{
				heap_remove_at(data->waiters, best);
				request->granted = 1;
				log_dongle_acquisition(request->s_coder);
				pthread_cond_broadcast(&data->scheduler_cond);
			}
			else
			{
				pthread_mutex_unlock(&data->scheduler_mutex);
				usleep(1000);
				continue ;
			}
		}
		else
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			usleep(1000);
			continue ;
		}
		pthread_mutex_unlock(&data->scheduler_mutex);
	}
	return (NULL);
}

/*
static int	dongle_is_available(t_dongle *dongle, long now)
{
	int	available;

	pthread_mutex_lock(&dongle->mutex);
	available = (dongle->in_use == 0 && dongle->available_at <= now);
	pthread_mutex_unlock(&dongle->mutex);
	return (available);
}
	*/

int	try_reserve_dongles(t_request *request, long now)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;
	t_dongle	*first;
	t_dongle	*second;
	int			available;

	coder = request->s_coder;
	left = coder->left_dongle;
	right = coder->right_dongle;
	if (left == right)
	{
		pthread_mutex_lock(&left->mutex);
		available = (left->in_use == 0 && left->available_at <= now);
		if (available)
			left->in_use = 1;
		pthread_mutex_unlock(&left->mutex);
		return (available);
	}
	first = left;
	second = right;
	if (first->id > second->id)
	{
		first = right;
		second = left;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	available = (left->in_use == 0 && left->available_at <= now
			&& right->in_use == 0 && right->available_at <= now);
	if (available)
	{
		left->in_use = 1;
		right->in_use = 1;
	}
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (available);
}

void	release_dongles(t_request *request)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;
	t_dongle	*first;
	t_dongle	*second;
	long		available_at;
	t_data		*data;

	coder = request->s_coder;
	data = coder->data;
	left = coder->left_dongle;
	right = coder->right_dongle;
	available_at = get_time_ms() + data->args.dongle_cooldown;
	if (left == right)
	{
		pthread_mutex_lock(&left->mutex);
		left->in_use = 0;
		left->available_at = available_at;
		pthread_mutex_unlock(&left->mutex);
	}
	else
	{
		first = left;
		second = right;
		if (first->id > second->id)
		{
			first = right;
			second = left;
		}
		pthread_mutex_lock(&first->mutex);
		pthread_mutex_lock(&second->mutex);
		left->in_use = 0;
		left->available_at = available_at;
		right->in_use = 0;
		right->available_at = available_at;
		pthread_mutex_unlock(&second->mutex);
		pthread_mutex_unlock(&first->mutex);
	}
	pthread_mutex_lock(&data->scheduler_mutex);
	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
}

static int	find_best_eligible_request(t_data *data, long now)
{
	int			i;
	int			best;
	t_request	*request;

	i = 0;
	best = -1;
	while (i < data->waiters->size)
	{
		request = data->waiters->entries[i];
		if (request != NULL && dongles_are_available(request, now))
		{
			if (best == -1 || data->waiters->compare(request,
					data->waiters->entries[best]) < 0)
				best = i;
		}
		i++;
	}
	return (best);
}

int	dongles_are_available(t_request *request, long now)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;
	t_dongle	*first;
	t_dongle	*second;
	int			available;

	coder = request->s_coder;
	left = coder->left_dongle;
	right = coder->right_dongle;
	if (left == right)
	{
		pthread_mutex_lock(&left->mutex);
		available = (left->in_use == 0 && left->available_at <= now);
		pthread_mutex_unlock(&left->mutex);
		return (available);
	}
	first = left;
	second = right;
	if (first->id > second->id)
	{
		first = right;
		second = left;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	available = (left->in_use == 0 && left->available_at <= now
			&& right->in_use == 0 && right->available_at <= now);
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (available);
}

static void	log_one_dongle(t_coder *coder)
{
	long	timestamp;

	timestamp = get_time_ms() - coder->data->start_time;
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld Coder [%d] has taken a dongle\n", timestamp, coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

static void	log_dongle_acquisition(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	log_one_dongle(coder);
	if (second != first)
		log_one_dongle(coder);
}
