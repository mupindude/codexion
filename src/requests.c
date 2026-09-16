/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 08:03:33 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/16 08:26:23 by dmupindu         ###   ########.fr       */
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
	if (heap_push(data->waiters, request))
	{
		free(request);
		return (1);
	}
	return (0);
}