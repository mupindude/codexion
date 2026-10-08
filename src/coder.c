/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 07:34:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/08 08:45:41 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void perform_compile(t_coder *coder);
static void perform_debug(t_coder *coder);
static void perform_refactor(t_coder *coder);
static int is_stopped(t_data *data);
static int	all_coders_finished(t_data *data); //temp check to see if program terminates after nb_compiles is reached
void *coder_routine(void *arg)
{
    t_coder    *coder;
    t_data     *data;
    t_request  *request;
    long       now;

    coder = (t_coder *)arg;
    data = coder->data;

    while (coder->compile_count < data->args.nb_compiles
        && is_stopped(data) == 0)
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
            coder->request = NULL;
            pthread_mutex_unlock(&data->scheduler_mutex);
            free(request);
            break ;
        }

        request = coder->request;
        coder->request = NULL;
        pthread_mutex_unlock(&data->scheduler_mutex);

        pthread_mutex_lock(&data->print_mutex);
        printf("Coder %d received permission to compile\n", coder->id);
        pthread_mutex_unlock(&data->print_mutex);

        perform_compile(coder);
        release_dongles(request);

		        free(request);

        perform_debug(coder);
        perform_refactor(coder);


		// testing if programm stops after all coders have reached nb_compiles
		if (all_coders_finished(data))
		{
			pthread_mutex_lock(&data->scheduler_mutex);
			data->stop = 1;
			pthread_cond_broadcast(&data->scheduler_cond);
			pthread_mutex_unlock(&data->scheduler_mutex);
		}

    }

    return (NULL);
}

static void perform_compile(t_coder *coder)
{
    t_data  *data;

    data = coder->data;
	pthread_mutex_lock(&data->scheduler_mutex);
    coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&data->scheduler_mutex);

    pthread_mutex_lock(&data->print_mutex);
    printf("Coder %d is compiling\n", coder->id);
    pthread_mutex_unlock(&data->print_mutex);

	usleep(data->args.time_to_compile * 1000);

	pthread_mutex_lock(&data->scheduler_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&data->scheduler_mutex);
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

static int is_stopped(t_data *data)
{
    int stop;

    pthread_mutex_lock(&data->scheduler_mutex);
    stop = data->stop;
    pthread_mutex_unlock(&data->scheduler_mutex);
    return (stop);
}

static int	all_coders_finished(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		if (data->coders[i].compile_count
			< data->args.nb_compiles)
			return (0);
		i++;
	}
	return (1);
}
