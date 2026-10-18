/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scheduler_blocking.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 07:32:40 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/18 12:51:09 by dmupindu         ###   ########.fr       */
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


static int test_fifo_eligible_priority(void)
{
    t_args      args;
    t_data      data;
    pthread_t   scheduler_thread;
    int         passed;
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

    /* Block Coder 1 on dongle 0. */
    pthread_mutex_lock(&data.dongles[0].mutex);
    data.dongles[0].in_use = 1;
    pthread_mutex_unlock(&data.dongles[0].mutex);

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

    passed = wait_for_grant(&data, 2, 200);

    /* Stop the scheduler before checking final results. */
    pthread_mutex_lock(&data.scheduler_mutex);
    data.stop = 1;
    pthread_cond_broadcast(&data.scheduler_cond);
    pthread_mutex_unlock(&data.scheduler_mutex);

	pthread_join(scheduler_thread, NULL);

    pthread_mutex_lock(&data.scheduler_mutex);
    if (data.coders[0].request != NULL
        && data.coders[0].request->granted != 0)
        passed = 0;
    if (data.coders[2].request != NULL
        && data.coders[2].request->granted != 0)
        passed = 0;
    pthread_mutex_unlock(&data.scheduler_mutex);

    /*
     * Granted requests have been removed from the heap.
     * Free those separately; destroy_data handles queued requests.
     */
    i = 0;
    while (i < data.args.nb_coders)
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

    if (passed)
        printf("PASS: FIFO chose the earliest eligible request.\n");
    else
        printf("FAIL: FIFO eligible-request priority test.\n");
    return (!passed);
}


static int test_edf_eligible_priority(void)
{
    t_args      args;
    t_data      data;
    pthread_t   scheduler_thread;
    long        now;
    int         passed;
    int         i;

    args.nb_coders = 4;
    args.time_to_burnout = 5000;
    args.time_to_compile = 100;
    args.time_to_debug = 100;
    args.time_to_refactor = 100;
    args.nb_compiles = 1;
    args.dongle_cooldown = 50;
    args.scheduler = EDF;

    if (init_data(&data, &args) != 0)
        return (1);

    now = get_time_ms();

    /* Earlier last_compile means an earlier EDF deadline. */
    data.coders[0].last_compile = now - 3000;
    data.coders[1].last_compile = now - 2000;
    data.coders[2].last_compile = now - 1000;

    /* Block Coder 1 on dongle 0. */
    pthread_mutex_lock(&data.dongles[0].mutex);
    data.dongles[0].in_use = 1;
    pthread_mutex_unlock(&data.dongles[0].mutex);

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

    passed = wait_for_grant(&data, 2, 200);

    pthread_mutex_lock(&data.scheduler_mutex);
    data.stop = 1;
    pthread_cond_broadcast(&data.scheduler_cond);
    pthread_mutex_unlock(&data.scheduler_mutex);

    pthread_join(scheduler_thread, NULL);

    pthread_mutex_lock(&data.scheduler_mutex);
    if (data.coders[0].request != NULL
        && data.coders[0].request->granted != 0)
        passed = 0;
    if (data.coders[2].request != NULL
        && data.coders[2].request->granted != 0)
        passed = 0;
    pthread_mutex_unlock(&data.scheduler_mutex);

    /*
     * Granted requests have been popped from the heap.
     * Free those separately; destroy_data handles queued requests.
     */
    i = 0;
    while (i < data.args.nb_coders)
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

    if (passed)
        printf("PASS: EDF chose the earliest-deadline eligible request.\n");
    else
        printf("FAIL: EDF eligible-request priority test.\n");
    return (!passed);
}


static int test_multiple_blocked_requests(void)
{
    t_args      args;
    t_data      data;
    pthread_t   scheduler_thread;
    int         passed;
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

    /* Block Coder 1 and Coder 2 using dongles 0 and 1. */
    pthread_mutex_lock(&data.dongles[0].mutex);
    data.dongles[0].in_use = 1;
    pthread_mutex_unlock(&data.dongles[0].mutex);

    pthread_mutex_lock(&data.dongles[1].mutex);
    data.dongles[1].in_use = 1;
    pthread_mutex_unlock(&data.dongles[1].mutex);

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

    passed = wait_for_grant(&data, 3, 200);

    pthread_mutex_lock(&data.scheduler_mutex);
    data.stop = 1;
    pthread_cond_broadcast(&data.scheduler_cond);
    pthread_mutex_unlock(&data.scheduler_mutex);

    pthread_join(scheduler_thread, NULL);

    pthread_mutex_lock(&data.scheduler_mutex);
    if (data.coders[0].request == NULL
		|| data.coders[1].request == NULL
		|| data.coders[0].request->granted != 0
		|| data.coders[1].request->granted != 0)
		passed = 0;
    pthread_mutex_unlock(&data.scheduler_mutex);

    /*
     * Free granted requests separately because they are no
     * longer owned by the heap. destroy_data handles queued ones.
     */
    i = 0;
    while (i < data.args.nb_coders)
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

    if (passed)
        printf("PASS: Scheduler skipped multiple blocked requests.\n");
    else
        printf("FAIL: Scheduler did not skip multiple blocked requests.\n");
    return (!passed);
}


static int test_blocked_request_becomes_eligible(void)
{
    t_args      args;
    t_data      data;
    pthread_t   scheduler_thread;
    int         passed;
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

    /* Keep Coder 1's left dongle busy. */
    pthread_mutex_lock(&data.dongles[0].mutex);
    data.dongles[0].in_use = 1;
    pthread_mutex_unlock(&data.dongles[0].mutex);

    if (submit_request(&data, &data.coders[0], 10) != 0)
    {
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

    /* Coder 1 should remain blocked while dongle 0 is busy. */
    passed = !wait_for_grant(&data, 1, 100);

    /* Release dongle 0 using its own mutex. */
    pthread_mutex_lock(&data.dongles[0].mutex);
    data.dongles[0].in_use = 0;
    pthread_mutex_unlock(&data.dongles[0].mutex);

    /* The scheduler should now be able to grant Coder 1. */
    if (!wait_for_grant(&data, 1, 300))
        passed = 0;

    pthread_mutex_lock(&data.scheduler_mutex);
    data.stop = 1;
    pthread_cond_broadcast(&data.scheduler_cond);
    pthread_mutex_unlock(&data.scheduler_mutex);

    pthread_join(scheduler_thread, NULL);

    /*
     * A granted request has been removed from the heap.
     * Free it separately; destroy_data frees requests still queued.
     */
    i = 0;
    while (i < data.args.nb_coders)
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

    if (passed)
        printf("PASS: Blocked request became eligible and was granted.\n");
    else
        printf("FAIL: Blocked request did not recover correctly.\n");
    return (!passed);
}


int main(void)
{
    t_args      args;
    t_data      data;
    pthread_t   scheduler_thread;
    int         coder3_granted;
    int         i;
	int			failed;

	failed = 0;
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
    while (i < data.args.nb_coders)
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
        printf("PASS: Coder 3 proceeded despite Coder 1 being blocked.\n");
    else
    {
        printf("FAIL: Coder 3 did not proceed while Coder 1 was blocked.\n");
        failed = 1;
    }

    if (test_fifo_eligible_priority() != 0)
        failed = 1;
    if (test_edf_eligible_priority() != 0)
        failed = 1;
    if (test_multiple_blocked_requests() != 0)
        failed = 1;
    if (test_blocked_request_becomes_eligible() != 0)
        failed = 1;

    return (failed);
}