/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 13:39:03 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/23 07:49:59 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	find_best_eligible_request(t_data *data, long now);
static void	log_one_dongle(t_coder *coder);
static void	log_dongle_acquisition(t_coder *coder);
static int	process_best_request(t_data *data, int best);

static int	process_best_request(t_data *data, int best)
{
	t_request	*request;

	request = data->waiters->entries[best];
	if (!try_reserve_dongles(request, get_time_ms()))
		return (0);
	heap_remove_at(data->waiters, best);
	request->granted = 1;
	log_dongle_acquisition(request->s_coder);
	pthread_cond_broadcast(&data->scheduler_cond);
	return (1);
}

void	*scheduler_routine(void *arg)
{
	t_data	*data;
	int		best;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->scheduler_mutex);
		while (data->waiters->size == 0 && data->stop == 0)
			pthread_cond_wait(&data->scheduler_cond,
				&data->scheduler_mutex);
		if (data->stop)
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			break ;
		}
		best = find_best_eligible_request(data, get_time_ms());
		if (best == -1 || !process_best_request(data, best))
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			usleep(1000);
			continue ;
		}
		pthread_mutex_unlock(&data->scheduler_mutex);
	}
	return (NULL);
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
