/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 13:55:44 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/25 15:47:53 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	destroy_global_sync(t_data *data)
{
	pthread_cond_destroy(&data->scheduler_cond);
	pthread_mutex_destroy(&data->scheduler_mutex);
	pthread_mutex_destroy(&data->print_mutex);
}

int	init_global_sync(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->scheduler_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	if (pthread_cond_init(&data->scheduler_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&data->scheduler_mutex);
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	return (0);
}

int	init_arrays(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * data->args.nb_coders);
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * data->args.nb_coders);
	if (!data->dongles)
	{
		free(data->coders);
		data->coders = NULL;
		return (1);
	}
	return (0);
}

int	init_waiters(t_data *data)
{
	int	(*compare)(t_request *a, t_request *b);

	if (data->args.scheduler == FIFO)
		compare = compare_fifo;
	else
		compare = compare_edf;
	data->waiters = malloc(sizeof(t_heap));
	if (!data->waiters)
		return (1);
	if (heap_init(data->waiters, data->args.nb_coders, compare))
	{
		free(data->waiters);
		data->waiters = NULL;
		return (1);
	}
	return (0);
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
