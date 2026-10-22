/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 08:10:44 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 08:51:14 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	join_threads(t_data *data, int created_coders)
{
	int	i;

	i = 0;
	while (i < created_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	pthread_mutex_lock(&data->scheduler_mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
	pthread_join(data->scheduler, NULL);
	pthread_join(data->monitor, NULL);
}
