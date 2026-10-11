/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 12:16:45 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/11 13:54:25 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void *monitor_routine(void *arg)
{
    t_data  *data;
    long    now;
    long    deadline;
    int     i;
    int     coder_id;

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
            deadline = data->coders[i].last_compile
                + data->args.time_to_burnout;
            if (data->stop == 0
                && data->coders[i].compile_count
                    < data->args.nb_compiles
                && now >= deadline)
            {
                coder_id = data->coders[i].id;
                data->stop = 1;
                pthread_cond_broadcast(&data->scheduler_cond);
                pthread_mutex_unlock(&data->scheduler_mutex);

                pthread_mutex_lock(&data->print_mutex);
                printf("%ld %d burned out\n", get_time_ms(), coder_id);
                printf("Deadline: %ld | Detected: %ld | Delay: %ld ms\n",
                    deadline, now, now - deadline);
                pthread_mutex_unlock(&data->print_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&data->scheduler_mutex);
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}