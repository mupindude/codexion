/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 07:34:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/20 13:51:28 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static t_request	*wait_for_request(t_coder *coder);
static int			run_coder_cycle(t_coder *coder);
static int			is_stopped(t_data *data);
static int			should_continue(t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (should_continue(coder))
	{
		if (!run_coder_cycle(coder))
			break ;
	}
	return (NULL);
}

static t_request	*wait_for_request(t_coder *coder)
{
	t_data		*data;
	t_request	*request;
	int			granted;

	data = coder->data;
	pthread_mutex_lock(&data->scheduler_mutex);
	while (coder->request->granted == 0 && data->stop == 0)
		pthread_cond_wait(&data->scheduler_cond, &data->scheduler_mutex);
	request = coder->request;
	coder->request = NULL;
	if (data->stop != 0)
	{
		granted = (request != NULL && request->granted != 0);
		pthread_mutex_unlock(&data->scheduler_mutex);
		if (granted)
		{
			release_dongles(request);
			free(request);
		}
		return (NULL);
	}
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (request);
}

static int	run_coder_cycle(t_coder *coder)
{
	t_request	*request;

	if (submit_request(coder->data, coder, get_time_ms()) != 0)
		return (0);
	request = wait_for_request(coder);
	if (request == NULL)
		return (0);
	if (!perform_compile(coder))
	{
		release_dongles(request);
		free(request);
		return (0);
	}
	release_dongles(request);
	free(request);
	if (is_stopped(coder->data))
		return (0);
	perform_debug(coder);
	if (is_stopped(coder->data))
		return (0);
	perform_refactor(coder);
	return (1);
}

static int	is_stopped(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->scheduler_mutex);
	stop = data->stop;
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (stop);
}

static int	should_continue(t_coder *coder)
{
	int		result;
	t_data	*data;

	data = coder->data;
	pthread_mutex_lock(&data->scheduler_mutex);
	result = (coder->compile_count < data->args.nb_compiles && data->stop == 0);
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (result);
}
