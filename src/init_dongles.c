/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 13:45:13 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 13:48:49 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	destroy_dongles(t_data *data, int count)
{
	while (count > 0)
	{
		count--;
		pthread_mutex_destroy(&data->dongles[count].mutex);
	}
}

int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].in_use = 0;
		data->dongles[i].available_at = 0;
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
		{
			destroy_dongles(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}
