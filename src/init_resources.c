/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 13:55:44 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 13:57:15 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	destroy_global_sync(t_data *data)
{
	pthread_cond_destroy(&data->scheduler_cond);
	pthread_mutex_destroy(&data->scheduler_mutex);
	pthread_mutex_destroy(&data->print_mutex);
}

int	init_resources(t_data *data)
{
	if (init_global_sync(data))
		return (1);
	if (init_waiters(data))
	{
		destroy_global_sync(data);
		return (1);
	}
	if (init_dongles(data))
	{
		destroy_heap(data->waiters);
		free(data->waiters);
		data->waiters = NULL;
		destroy_global_sync(data);
		return (1);
	}
	return (0);
}
