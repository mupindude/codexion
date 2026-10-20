/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 08:17:07 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/20 08:23:52 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	update_completion(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		if (data->coders[i].compile_count < data->args.nb_compiles)
			return ;
		i++;
	}
	data->stop = 1;
	pthread_cond_broadcast(&data->scheduler_cond);
}

int	perform_compile(t_coder *coder)
{
	t_data	*data;

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
	update_completion(data);
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (1);
}
