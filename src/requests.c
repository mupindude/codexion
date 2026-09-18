/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 08:03:33 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/18 13:45:43 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

t_request	*create_request(t_coder *coder, long now, int time_to_burnout)
{
	t_request	*request;

	request = malloc(sizeof(t_request));
	if (!request)
		return (NULL);
	request->s_coder = coder;
	request->arrival_time = now;
	request->deadline = now + time_to_burnout;
	return (request);
}

int	submit_request(t_data *data, t_coder *coder, long now)
{
	t_request	*request;

	request = create_request(coder, now,
			data->args.time_to_burnout);
	if (!request)
		return (1);
	pthread_mutex_lock(&data->scheduler_mutex);
	if (heap_push(data->waiters, request))
	{
		pthread_mutex_unlock(&data->scheduler_mutex);
		free(request);
		return (1);
	}
	pthread_cond_signal(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (0);
}

t_request	*get_next_request(t_data *data)
{
	return (heap_pop(data->waiters));
}