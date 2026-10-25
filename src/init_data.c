/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:14:25 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/25 12:43:23 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_data(t_data *data, t_args *args)
{
	data->args = *args;
	data->stop = 0;
	data->start_time = get_time_ms();
	data->waiters = NULL;
	data->coders = NULL;
	data->dongles = NULL;
	if (init_arrays(data))
		return (1);
	if (init_resources(data))
	{
		free(data->coders);
		free(data->dongles);
		data->coders = NULL;
		data->dongles = NULL;
		return (1);
	}
	if (init_coders(data))
	{
		destroy_data(data);
		return (1);
	}
	return (0);
}

int	init_coders(t_data *data)
{
	int	i;
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
		data->coders[i].request = NULL;
		i++;
	}
	return (0);
}

void	destroy_data(t_data *data)
{
	int	i;

	if (data == NULL)
		return ;
	if (data->waiters)
	{
		while (data->waiters->size > 0)
			free(heap_pop(data->waiters));
		destroy_heap(data->waiters);
		free(data->waiters);
		data->waiters = NULL;
	}
	i = 0;
	while (i < data->args.nb_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->scheduler_mutex);
	pthread_cond_destroy(&data->scheduler_cond);
	free(data->coders);
	data->coders = NULL;
	free(data->dongles);
	data->dongles = NULL;
}
