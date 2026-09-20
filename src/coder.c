/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 07:34:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/20 17:33:21 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_data		*data;
	long	now;

	coder = (t_coder *)arg;
	data = coder->data;
	now = 0;

	pthread_mutex_lock(&data->scheduler_mutex);
	coder->scheduled = 0;
	pthread_mutex_unlock(&data->scheduler_mutex);

	if (submit_request(data, coder, now) != 0)
		return (NULL);

	pthread_mutex_lock(&data->scheduler_mutex);
	while (coder->scheduled == 0 && data->stop == 0)
		pthread_cond_wait(&data->scheduler_cond,
			&data->scheduler_mutex);
	pthread_mutex_unlock(&data->scheduler_mutex);

	if (data->stop != 0)
		return (NULL);

	pthread_mutex_lock(&data->print_mutex);
	printf("Coder %d received permission to compile\n", coder->id);
	pthread_mutex_unlock(&data->print_mutex);

	return (NULL);
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
