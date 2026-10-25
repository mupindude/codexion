/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scheduler_live.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 07:30:47 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/25 12:26:39 by dmupindu         ###   ########.fr       */
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

static int	wait_for_grant(t_data *data, int expected_coder)
{
	long	start;
	int		granted_coder;
	int		i;

	start = get_time_ms();
	while (get_time_ms() - start < 1000)
	{
		granted_coder = 0;
		pthread_mutex_lock(&data->scheduler_mutex);
		i = 0;
		while (i < 3)
		{
			if (data->coders[i].request != NULL
				&& data->coders[i].request->granted != 0)
			{
				granted_coder = data->coders[i].id;
				break ;
			}
			i++;
		}
		pthread_mutex_unlock(&data->scheduler_mutex);
		if (granted_coder != 0)
			return (granted_coder == expected_coder);
		usleep(1000);
	}
	return (0);
}

static int	run_scheduler_test(t_scheduler scheduler)
{
	t_args		args;
	t_data		data;
	pthread_t	scheduler_thread;
	int			result;
	//int			i;

	args.nb_coders = 4;
	args.time_to_burnout = 1000;
	args.time_to_compile = 100;
	args.time_to_debug = 100;
	args.time_to_refactor = 100;
	args.nb_compiles = 1;
	args.dongle_cooldown = 50;
	args.scheduler = scheduler;
	if (init_data(&data, &args) != 0)
		return (1);
	if (scheduler == EDF)
		set_deadlines(&data);
	if (scheduler == EDF)
	{
		if (submit_request(&data, &data.coders[0], 10) != 0
        || submit_request(&data, &data.coders[1], 20) != 0
        || submit_request(&data, &data.coders[2], 30) != 0)
		{
			pthread_mutex_lock(&data.scheduler_mutex);
			data.stop = 1;
			pthread_cond_broadcast(&data.scheduler_cond);
			pthread_mutex_unlock(&data.scheduler_mutex);
			destroy_data(&data);
			return (1);
		}
	}
	else
	{
		if (submit_request(&data, &data.coders[0], 10) != 0
        || submit_request(&data, &data.coders[1], 20) != 0
        || submit_request(&data, &data.coders[2], 30) != 0)
		{
			pthread_mutex_lock(&data.scheduler_mutex);
			data.stop = 1;
			pthread_cond_broadcast(&data.scheduler_cond);
			pthread_mutex_unlock(&data.scheduler_mutex);
			destroy_data(&data);
			return (1);
		}
	}
	if (pthread_create(&scheduler_thread, NULL, scheduler_routine, &data) != 0)
	{
		pthread_mutex_lock(&data.scheduler_mutex);
		data.stop = 1;
		pthread_cond_broadcast(&data.scheduler_cond);
		pthread_mutex_unlock(&data.scheduler_mutex);
		destroy_data(&data);
		return (1);
	}
	if (scheduler == EDF)
    	result = wait_for_grant(&data, 2);
	else
		result = wait_for_grant(&data, 1);
	pthread_mutex_lock(&data.scheduler_mutex);
	data.stop = 1;
	pthread_cond_broadcast(&data.scheduler_cond);
	pthread_mutex_unlock(&data.scheduler_mutex);
	pthread_join(scheduler_thread, NULL);
	// i = 0;
	// while (i < 3)
	// {
	//     if (data.coders[i].request != NULL)
	//     {
	//         free(data.coders[i].request);
	//         data.coders[i].request = NULL;
	//     }
	//     i++;
	// }
	destroy_data(&data);
	if (result)
		printf("%s test passed.\n", scheduler == EDF ? "EDF" : "FIFO");
	else
		printf("%s test failed.\n", scheduler == EDF ? "EDF" : "FIFO");
	return (!result);
}

int	main(void)
{
	if (run_scheduler_test(EDF) != 0)
		return (1);
	if (run_scheduler_test(FIFO) != 0)
		return (1);
	return (0);
}