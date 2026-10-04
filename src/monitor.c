/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 12:16:45 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/04 17:44:10 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void *monitor_routine(void *arg)
{
    t_data  *data;
    long    now;
    int     i;

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

        now = get_time_ms();

        i = 0;
        while (i < data->args.nb_coders)
		{
			pthread_mutex_lock(&data->scheduler_mutex);

    		if (data->coders[i].compile_count < data->args.nb_compiles
        		&& data->coders[i].request != NULL
        		&& now >= data->coders[i].request->deadline)
    		{
				pthread_mutex_unlock(&data->scheduler_mutex);

				pthread_mutex_lock(&data->print_mutex);
				printf("Coder %d burned out\n", data->coders[i].id);
				printf("Deadline: %ld | Detected: %ld | Delay: %ld ms\n",
					data->coders[i].request->deadline,
					now,
					now - data->coders[i].request->deadline);
				pthread_mutex_unlock(&data->print_mutex);

				pthread_mutex_lock(&data->scheduler_mutex);
				data->stop = 1;
				pthread_cond_broadcast(&data->scheduler_cond);
				pthread_mutex_unlock(&data->scheduler_mutex);
				return (NULL);
    		}

    		pthread_mutex_unlock(&data->scheduler_mutex);
    		i++;
		}
        usleep(1000);
    }
    return (NULL);
}