/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_compare.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 07:30:45 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 07:34:23 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	compare_fifo(t_request *a, t_request *b)
{
	if (a->arrival_time < b->arrival_time)
		return (-1);
	if (a->arrival_time > b->arrival_time)
		return (1);
	if (a->s_coder->id < b->s_coder->id)
		return (-1);
	if (a->s_coder->id > b->s_coder->id)
		return (1);
	return (0);
}

int	compare_edf(t_request *a, t_request *b)
{
	if (a->deadline < b->deadline)
		return (-1);
	if (a->deadline > b->deadline)
		return (1);
	if (a->s_coder->id < b->s_coder->id)
		return (-1);
	if (a->s_coder->id > b->s_coder->id)
		return (1);
	return (0);
}
