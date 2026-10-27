/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 08:09:46 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/27 08:32:07 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	log_state(t_coder *coder, const char *message)
{
	long	timestamp;

	pthread_mutex_lock(&coder->data->print_mutex);
	timestamp = get_time_ms() - coder->data->start_time;
	printf("%ld Coder [%d] %s\n", timestamp, coder->id, message);
	pthread_mutex_unlock(&coder->data->print_mutex);
}
