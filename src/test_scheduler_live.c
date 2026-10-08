/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scheduler_live.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 07:30:47 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/08 07:32:29 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	set_deadlines(t_data *data)
{
	pthread_mutex_lock(&data->scheduler_mutex);

	data->coders[0].last_compile = 4000;
	data->coders[1].last_compile = 1000;
	data->coders[2].last_compile = 2000;

	pthread_mutex_unlock(&data->scheduler_mutex);
}

int	main(void)
{
	t_args		args;
	t_data		data;
	pthread_t	scheduler_thread;
	int			i;

	args.nb_coders = 4;
	args.time_to_burnout = 1000;
	args.time_to_compile = 100;
	args.time_to_debug = 100;
	args.time_to_refactor = 100;
	args.nb_compiles = 1;
	args.dongle_cooldown = 50;
	args.scheduler = EDF;

	if (init_data(&data, &args) != 0)
		return (1);

	set_deadlines(&data);

	submit_request(&data, &data.coders[0], 10);
	submit_request(&data, &data.coders[1], 20);
	submit_request(&data, &data.coders[2], 30);

	if (pthread_create(&scheduler_thread, NULL,
			scheduler_routine, &data) != 0)
	{
		destroy_data(&data);
		return (1);
	}

	while (1)
	{
		pthread_mutex_lock(&data.scheduler_mutex);
		if (data.coders[1].request != NULL
			&& data.coders[1].request->granted != 0)
		{
			pthread_mutex_unlock(&data.scheduler_mutex);
			break ;
		}
		pthread_mutex_unlock(&data.scheduler_mutex);
		usleep(1000);
	}

	printf("\nEDF selected the earliest deadline first.\n");

	pthread_mutex_lock(&data.scheduler_mutex);
	data.stop = 1;
	pthread_cond_broadcast(&data.scheduler_cond);
	pthread_mutex_unlock(&data.scheduler_mutex);

	pthread_join(scheduler_thread, NULL);

	i = 0;
	while (i < 3)
	{
		if (data.coders[i].request != NULL)
		{
			free(data.coders[i].request);
			data.coders[i].request = NULL;
		}
		i++;
	}

	destroy_data(&data);
	return (0);
}