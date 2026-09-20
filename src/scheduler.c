/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 13:39:03 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/20 14:13:55 by dmupindu         ###   ########.fr       */
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
		while ( data->waiters->size == 0 && data->stop == 0)
			pthread_cond_wait(&data->scheduler_cond,
				&data->scheduler_mutex);
		if (data->stop && data->waiters->size == 0)
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			break;
		}

		request = heap_pop(data->waiters);
		pthread_mutex_unlock(&data->scheduler_mutex);
		if (request != NULL)
		{
			pthread_mutex_lock(&data->print_mutex);
			printf("Scheduler selected Coder %d\n",
				request->s_coder->id);
			pthread_mutex_unlock(&data->print_mutex);
			free(request);
		}
	}
	return (NULL);
}