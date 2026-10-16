/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_dongle_cooldown.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 07:28:28 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/16 07:29:43 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int check_dongle_state(t_dongle *dongle, int expected)
{
    int result;

    pthread_mutex_lock(&dongle->mutex);
    result = (dongle->in_use == expected);
    pthread_mutex_unlock(&dongle->mutex);
    return (result);
}

int main(void)
{
    t_args      args;
    t_data      data;
    t_request   request;
    t_dongle    *left;
    t_dongle    *right;

    args.nb_coders = 4;
    args.time_to_burnout = 2000;
    args.time_to_compile = 100;
    args.time_to_debug = 100;
    args.time_to_refactor = 100;
    args.nb_compiles = 1;
    args.dongle_cooldown = 500;
    args.scheduler = FIFO;

    if (init_data(&data, &args) != 0)
        return (1);

    request.s_coder = &data.coders[0];
    left = data.coders[0].left_dongle;
    right = data.coders[0].right_dongle;

    /*
     * Both dongles are free, but the right dongle
     * is unavailable until timestamp 1500.
     */
    pthread_mutex_lock(&left->mutex);
    left->in_use = 0;
    left->available_at = 1000;
    pthread_mutex_unlock(&left->mutex);

    pthread_mutex_lock(&right->mutex);
    right->in_use = 0;
    right->available_at = 1500;
    pthread_mutex_unlock(&right->mutex);

    /* Test 1: one dongle is still cooling down. */
    if (try_reserve_dongles(&request, 1499) != 0)
    {
        printf("FAIL: reserved a dongle before cooldown expired.\n");
        destroy_data(&data);
        return (1);
    }
    if (!check_dongle_state(left, 0)
        || !check_dongle_state(right, 0))
    {
        printf("FAIL: dongle state changed after rejected reservation.\n");
        destroy_data(&data);
        return (1);
    }
    printf("PASS: reservation rejected before cooldown expires.\n");

    /* Test 2: both dongles are available at timestamp 1500. */
    if (try_reserve_dongles(&request, 1500) == 0)
    {
        printf("FAIL: reservation rejected at cooldown expiry.\n");
        destroy_data(&data);
        return (1);
    }
    if (!check_dongle_state(left, 1)
        || !check_dongle_state(right, 1))
    {
        printf("FAIL: both dongles were not reserved.\n");
        destroy_data(&data);
        return (1);
    }
    printf("PASS: reservation succeeds when cooldown expires.\n");

    /* Test 3: already-reserved dongles cannot be reserved again. */
    if (try_reserve_dongles(&request, 1501) != 0)
    {
        printf("FAIL: reserved dongles were granted twice.\n");
        destroy_data(&data);
        return (1);
    }
    printf("PASS: already-reserved dongles cannot be reserved again.\n");

    destroy_data(&data);
    return (0);
}