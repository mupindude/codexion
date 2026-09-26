/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 07:34:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/26 07:44:16 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void perform_compile(t_coder *coder);
static void perform_debug(t_coder *coder);
static void perform_refactor(t_coder *coder);

void *coder_routine(void *arg)
{
    t_coder *coder;
    t_data  *data;
    long    now;

    coder = (t_coder *)arg;
    data = coder->data;

	while (coder->compile_count < data->args.nb_compiles
        && data->stop == 0)
		{
			now = get_time_ms();

			if (submit_request(data, coder, now) != 0)
			return (NULL);

			pthread_mutex_lock(&data->scheduler_mutex);
			while (coder->request->granted == 0 && data->stop == 0)
        		pthread_cond_wait(&data->scheduler_cond,
            		&data->scheduler_mutex);
			pthread_mutex_unlock(&data->scheduler_mutex);
			if (data->stop != 0)
					break;

			pthread_mutex_lock(&data->print_mutex);
			printf("Coder %d received permission to compile\n", coder->id);
    		pthread_mutex_unlock(&data->print_mutex);

			perform_compile(coder);
    		release_dongles(coder->request);

			free(coder->request);
    		coder->request = NULL;

			perform_debug(coder);
			perform_refactor(coder);
		}
	pthread_mutex_lock(&data->print_mutex);
	printf("Coder %d completed %d compiles\n",
    coder->id, coder->compile_count);
	pthread_mutex_unlock(&data->print_mutex);

    return (NULL);
}

static void perform_compile(t_coder *coder)
{
    t_data  *data;

    data = coder->data;
    coder->last_compile = get_time_ms();

    pthread_mutex_lock(&data->print_mutex);
    printf("Coder %d is compiling\n", coder->id);
    pthread_mutex_unlock(&data->print_mutex);

    usleep(data->args.time_to_compile * 1000);

    coder->compile_count++;
}

static void perform_debug(t_coder *coder)
{
    t_data  *data;

    data = coder->data;

    pthread_mutex_lock(&data->print_mutex);
    printf("Coder %d is debugging\n", coder->id);
    pthread_mutex_unlock(&data->print_mutex);

    usleep(data->args.time_to_debug * 1000);
}

static void perform_refactor(t_coder *coder)
{
    t_data  *data;

    data = coder->data;
    pthread_mutex_lock(&data->print_mutex);
    printf("Coder %d is refactoring\n", coder->id);
    pthread_mutex_unlock(&data->print_mutex);
    usleep(data->args.time_to_refactor * 1000);
}

/*static	void take_dongle(t_dongle  *dongle)
{
	pthread_mutex_lock(&dongle->mutex);

	while (dongle->in_use)
	{
		pthread_mutex_unlock(&dongle->mutex);
		usleep(1000);
		pthread_mutex_lock(&dongle->mutex);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->mutex);
}

static	void take_dongles(t_coder *coder)
{
	take_dongle(coder->left_dongle);
	take_dongle(coder->right_dongle);
}

static void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	pthread_mutex_unlock(&dongle->mutex);
}

static void release_dongles(t_coder *coder)
{
	release_dongle(coder->left_dongle);
	release_dongle(coder->right_dongle);
} */
