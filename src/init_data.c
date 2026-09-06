/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:14:25 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/06 15:05:17 by dmupindu         ###   ########.fr       */
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
	{
		free(data->coders);
		free(data->dongles);
		return (1);
	}

	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		free(data->coders);
		free(data->dongles);
		return (1);
	}
	if (init_dongles(data))
		return (1);
	if (init_coders(data))
		return (1);
	return (0);
}

int	init_dongles(t_data *data)
{
	int i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].in_use = 0;
		data->dongles[i].available_at = 0;
		data->dongles[i].waiters = NULL;
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&data->dongles[i].mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int init_coders(t_data *data)
{
	int i;
	int	nb_coders;

	i = 0;
	nb_coders = data->args.nb_coders;
	while (i < nb_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compile_count = 0;
		data->coders[i].last_compile = data->start_time;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[(i + 1) % nb_coders];
		data->coders[i].data = data;
		i++;
	}
	return (0);
}

 void	destroy_data(t_data *data)
 {
	if (data == NULL)
		return ;
	free (data->coders);
	data->coders = NULL;
	free (data->dongles);
	data->dongles = NULL;
 }