/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 08:28:08 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 08:32:21 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	start_simulation(t_data *data)
{
	if (pthread_create(&data->scheduler, NULL,
			scheduler_routine, data) != 0)
	{
		destroy_data(data);
		return (1);
	}
	if (pthread_create(&data->monitor, NULL,
			monitor_routine, data) != 0)
	{
		pthread_mutex_lock(&data->scheduler_mutex);
		data->stop = 1;
		pthread_cond_broadcast(&data->scheduler_cond);
		pthread_mutex_unlock(&data->scheduler_mutex);
		pthread_join(data->scheduler, NULL);
		destroy_data(data);
		return (1);
	}
	return (0);
}
