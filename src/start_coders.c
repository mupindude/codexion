/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_coders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 07:57:41 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 08:00:42 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->scheduler_mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
}

int	start_coders(t_data *data, int *created_coders)
{
	int	i;

	*created_coders = 0;
	i = 0;
	while (i < data->args.nb_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL,
				coder_routine, &data->coders[i]) != 0)
		{
			stop_simulation(data);
			return (1);
		}
		(*created_coders)++;
		i++;
	}
	return (0);
}
