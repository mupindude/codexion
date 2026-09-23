/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 13:39:03 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/23 07:46:39 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	*scheduler_routine(void *arg)
{
	t_data		*data;
	t_request	*request;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->scheduler_mutex);
		while (data->waiters->size == 0 && data->stop == 0)
			pthread_cond_wait(&data->scheduler_cond,
				&data->scheduler_mutex);
		if (data->stop && data->waiters->size == 0)
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			break ;
		}

		request = heap_peek(data->waiters);
		if (request != NULL
			&& try_reserve_dongles(request, get_time_ms()))
		{
			request = heap_pop(data->waiters);
			request->granted = 1;

			pthread_mutex_lock(&data->print_mutex);
			printf("Scheduler selected Coder %d\n",
				request->s_coder->id);
			pthread_mutex_unlock(&data->print_mutex);

			pthread_cond_broadcast(&data->scheduler_cond);
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
static int dongle_is_available(t_dongle *dongle, long now)
{
	int	available;

	pthread_mutex_lock(&dongle->mutex);
	available = (dongle->in_use == 0 && dongle->available_at <= now);
	pthread_mutex_unlock(&dongle->mutex);
	return (available);
}
	*/

int try_reserve_dongles(t_request *request, long now)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;

	coder = request->s_coder;
	left = coder->left_dongle;
	right = coder->right_dongle;

	if (left == right)
	{
		if (left->in_use != 0 || left->available_at > now)
			return (0);
		left->in_use = 1;
		return (1);
	}
	if (left->in_use != 0 || left->available_at > now)
		return (0);
	if (right->in_use != 0 || right->available_at > now)
		return (0);
	left->in_use = 1;
	right->in_use = 1;
	return (1);
}

void	release_dongles(t_request *request)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;
	long		available_at;
	t_data		*data;

	coder = request->s_coder;
	data = coder->data;
	left = coder->left_dongle;
	right = coder->right_dongle;
	available_at = get_time_ms() + data->args.dongle_cooldown;

	pthread_mutex_lock(&data->scheduler_mutex);

	left->in_use = 0;
	left->available_at = available_at;
	if (left != right)
	{
		right->in_use = 0;
		right->available_at = available_at;
	}

	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
}