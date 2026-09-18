/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 07:34:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/18 08:17:46 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->data->args.nb_compiles)
	{
		printf("Coder %d is compiling\n", coder->id);
		usleep(coder->data->args.time_to_compile * 1000);

		printf("Coder %d is debugging\n", coder->id);
		usleep(coder->data->args.time_to_debug * 1000);

		printf("Coder %d is refactoring\n", coder->id);
		usleep(coder->data->args.time_to_refactor * 1000);

		coder->compile_count++;
		i++;
	}
	return(NULL);
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
