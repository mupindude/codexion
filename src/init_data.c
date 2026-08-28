/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:14:25 by dmupindu          #+#    #+#             */
/*   Updated: 2026/08/28 13:31:36 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_data(t_data *data, t_args *args)
{
	data->args = *args;
	data->stop = 0;
	data->start_time = 0;
	data->coders = malloc(sizeof(t_coder) * data->args.nb_coders);
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * data->args.nb_coders);
	if (!data->dongles)
	{
		free(data->coders);
		return (1);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	return (0);
}

void	init_dongles(t_data *data)
{
	int i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].in_use = 0;
		data->dongles[i].available_at = 0;
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		i++;
	}
}