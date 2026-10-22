/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 12:16:45 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 07:24:45 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	check_coder_burnout(t_data *data, int i)
{
	long	now;
	long	deadline;
	int		coder_id;

	now = get_time_ms();
	deadline = data->coders[i].last_compile
		+ data->args.time_to_burnout;
	if (data->stop != 0
		|| data->coders[i].compile_count >= data->args.nb_compiles
		|| now < deadline)
		return (0);
	coder_id = data->coders[i].id;
	data->stop = 1;
	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld Coder [%d] burned out\n", now - data->start_time,
		coder_id);
	printf("Deadline: %ld | Detected: %ld | Delay: %ld ms\n",
		deadline - data->start_time, now - data->start_time,
		now - deadline);
	pthread_mutex_unlock(&data->print_mutex);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->scheduler_mutex);
		if (data->stop != 0)
		{
			pthread_mutex_unlock(&data->scheduler_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->scheduler_mutex);
		i = 0;
		while (i < data->args.nb_coders)
		{
			pthread_mutex_lock(&data->scheduler_mutex);
			if (check_coder_burnout(data, i))
				return (NULL);
			pthread_mutex_unlock(&data->scheduler_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
