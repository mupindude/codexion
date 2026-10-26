/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 17:00:43 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/26 08:21:34 by dmupindu         ###   ########.fr       */
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

static int	all_coders_completed(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		if (data->coders[i].compile_count < data->args.nb_compiles)
			return (0);
		i++;
	}
	return (1);
}

void	finish_simulation(t_data *data)
{
	if (all_coders_completed(data))
		printf("=== Simulation over. All coders completed %d compiles ===\n",
			data->args.nb_compiles);
	destroy_data(data);
}
