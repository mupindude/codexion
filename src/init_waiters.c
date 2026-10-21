/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_waiters.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 08:32:34 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 08:39:57 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_waiters(t_data *data)
{
	int	(*compare)(t_request *a, t_request *b);

	if (data->args.scheduler == FIFO)
		compare = compare_fifo;
	else
		compare = compare_edf;
	data->waiters = malloc(sizeof(t_heap));
	if (!data->waiters)
		return (1);
	if (heap_init(data->waiters, data->args.nb_coders, compare))
	{
		free(data->waiters);
		data->waiters = NULL;
		return (1);
	}
	return (0);
}
