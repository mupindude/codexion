/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scheduler_blocking.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 07:32:40 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/16 07:48:37 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int wait_for_grant(t_data *data, int coder_id, long timeout_ms)
{
    long    start;
    int     granted;

    start = get_time_ms();
    while (get_time_ms() - start < timeout_ms)
    {
        pthread_mutex_lock(&data->scheduler_mutex);
        granted = (data->coders[coder_id - 1].request != NULL
                && data->coders[coder_id - 1].request->granted != 0);
        pthread_mutex_unlock(&data->scheduler_mutex);
        if (granted)
            return (1);
        usleep(1000);
    }
    return (0);
}

int main(void)
{
    t_args      args;
    t_data      data;
    pthread_t   scheduler_thread;
    int         coder3_granted;
    int         i;

    args.nb_coders = 4;
    args.time_to_burnout = 5000;
    args.time_to_compile = 100;
    args.time_to_debug = 100;
    args.time_to_refactor = 100;
    args.nb_compiles = 1;
    args.dongle_cooldown = 50;
    args.scheduler = FIFO;

    if (init_data(&data, &args) != 0)
        return (1);

    /*
     * Coder 1 needs dongles 0 and 1.
     * Keep dongle 0 busy so Coder 1 cannot be granted.
     */
    pthread_mutex_lock(&data.dongles[0].mutex);
    data.dongles[0].in_use = 1;
    pthread_mutex_unlock(&data.dongles[0].mutex);

    /*
     * Coder 1: highest priority, needs dongles 0 and 1.
     * Coder 2: next priority, needs dongles 1 and 2.
     * Coder 3: lowest priority, needs dongles 2 and 3.
     *
     * Coder 3's dongles are free and do not overlap
     * with Coder 1's dongles.
     */
    if (submit_request(&data, &data.coders[0], 10) != 0
        //|| submit_request(&data, &data.coders[1], 20) != 0
        || submit_request(&data, &data.coders[2], 30) != 0)
    {
        pthread_mutex_lock(&data.scheduler_mutex);
        data.stop = 1;
        pthread_cond_broadcast(&data.scheduler_cond);
        pthread_mutex_unlock(&data.scheduler_mutex);
        destroy_data(&data);
        return (1);
    }

    if (pthread_create(&scheduler_thread, NULL,
            scheduler_routine, &data) != 0)
    {
        pthread_mutex_lock(&data.scheduler_mutex);
        data.stop = 1;
        pthread_cond_broadcast(&data.scheduler_cond);
        pthread_mutex_unlock(&data.scheduler_mutex);
        destroy_data(&data);
        return (1);
    }

    /*
     * Give the scheduler 200 ms to grant Coder 3.
     */
    coder3_granted = wait_for_grant(&data, 3, 200);

    pthread_mutex_lock(&data.scheduler_mutex);
    data.stop = 1;
    pthread_cond_broadcast(&data.scheduler_cond);
    pthread_mutex_unlock(&data.scheduler_mutex);

    pthread_join(scheduler_thread, NULL);

    /*
     * Requests still in the heap are owned by the heap.
     * A granted request has been popped from the heap,
     * so free that request separately.
     */
    i = 0;
    while (i < 3)
    {
        if (data.coders[i].request != NULL)
        {
            if (data.coders[i].request->granted != 0)
                free(data.coders[i].request);
            data.coders[i].request = NULL;
        }
        i++;
    }

    destroy_data(&data);

    if (coder3_granted)
    {
        printf("PASS: Coder 3 proceeded despite Coder 1 being blocked.\n");
        return (0);
    }

    printf("HEAD-OF-LINE BLOCKING CONFIRMED:\n");
    printf("Coder 1 was blocked, and Coder 3 was not granted.\n");
    printf("The scheduler only examined the highest-priority request.\n");
    return (1);
}