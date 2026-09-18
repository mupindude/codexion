/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 13:39:03 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/18 13:41:53 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	*scheduler_routine(void *arg)
{
	t_data		*data;
	t_request	*request;

	data = (t_data *)arg;

	pthread_mutex_lock(&data->scheduler_mutex);

	request = heap_pop(data->waiters);
	if (request != NULL)
	{
		data->current_request = request;
		pthread_cond_broadcast(&data->scheduler_cond);
	}

	pthread_mutex_unlock(&data->scheduler_mutex);

	return (NULL);
}