/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 07:34:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/17 07:28:09 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int perform_compile(t_coder *coder);
static void perform_debug(t_coder *coder);
static void perform_refactor(t_coder *coder);
static int is_stopped(t_data *data);
static int should_continue(t_coder *coder);

void *coder_routine(void *arg)
{
    t_coder    *coder;
    t_data     *data;
    t_request  *request;
    long       now;
	int	granted;

    coder = (t_coder *)arg;
    data = coder->data;

    while (should_continue(coder))
    {
        now = get_time_ms();

        if (submit_request(data, coder, now) != 0)
            return (NULL);

        pthread_mutex_lock(&data->scheduler_mutex);
        while (coder->request->granted == 0 && data->stop == 0)
            pthread_cond_wait(&data->scheduler_cond,
                &data->scheduler_mutex);

        if (data->stop != 0)
        {
			 request = coder->request;
    		granted = (request != NULL && request->granted != 0);
            coder->request = NULL;
            pthread_mutex_unlock(&data->scheduler_mutex);
			if (granted)
			{
				release_dongles(request);
				free(request);
			}
            break ;
        }

        request = coder->request;
        coder->request = NULL;
        pthread_mutex_unlock(&data->scheduler_mutex);

		if (!perform_compile(coder))
		{
			release_dongles(request);
			free(request);
			break;
		}
		release_dongles(request);
		free(request);

		if (is_stopped(data))
			break;
        perform_debug(coder);
		if (is_stopped(data))
			break;
        perform_refactor(coder);
    }

    return (NULL);
}

static int perform_compile(t_coder *coder)
{
    t_data  *data;
    int     i;
    int     all_finished;

    data = coder->data;
    pthread_mutex_lock(&data->scheduler_mutex);
    if (data->stop)
    {
        pthread_mutex_unlock(&data->scheduler_mutex);
        return (0);
    }
    coder->last_compile = get_time_ms();
    pthread_mutex_unlock(&data->scheduler_mutex);

    log_state(coder, "is compiling");
    usleep(data->args.time_to_compile * 1000);

    pthread_mutex_lock(&data->scheduler_mutex);
    coder->compile_count++;

    i = 0;
    all_finished = 1;
    while (i < data->args.nb_coders)
    {
        if (data->coders[i].compile_count
            < data->args.nb_compiles)
        {
            all_finished = 0;
            break;
        }
        i++;
    }
    if (all_finished)
    {
        data->stop = 1;
        pthread_cond_broadcast(&data->scheduler_cond);
    }
    pthread_mutex_unlock(&data->scheduler_mutex);
    return (1);
}

static void perform_debug(t_coder *coder)
{
    t_data  *data;

    data = coder->data;

    log_state(coder, "is debugging");

    usleep(data->args.time_to_debug * 1000);
}

static void perform_refactor(t_coder *coder)
{
    t_data  *data;

    data = coder->data;
    log_state(coder, "is refactoring");
    usleep(data->args.time_to_refactor * 1000);
}

static int is_stopped(t_data *data)
{
    int stop;

    pthread_mutex_lock(&data->scheduler_mutex);
    stop = data->stop;
    pthread_mutex_unlock(&data->scheduler_mutex);
    return (stop);
}

/*
static int	all_coders_finished(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->scheduler_mutex);
	i = 0;
	while (i < data->args.nb_coders)
	{
		if (data->coders[i].compile_count
			< data->args.nb_compiles)
			{
				pthread_mutex_unlock(&data->scheduler_mutex);
				return (0);
			}
		i++;
	}
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (1);
}
*/

static int should_continue(t_coder *coder)
{
    int result;
    t_data *data;

    data = coder->data;
    pthread_mutex_lock(&data->scheduler_mutex);
    result = (coder->compile_count < data->args.nb_compiles
            && data->stop == 0);
    pthread_mutex_unlock(&data->scheduler_mutex);
    return (result);
}