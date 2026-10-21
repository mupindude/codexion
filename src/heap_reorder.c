/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_reorder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 07:40:11 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 07:47:11 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	heap_swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_sift_down(t_heap *heap, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		smallest = index;
		if (left < heap->size && heap->compare(heap->entries[left],
				heap->entries[smallest]) < 0)
			smallest = left;
		if (right < heap->size && heap->compare(heap->entries[right],
				heap->entries[smallest]) < 0)
			smallest = right;
		if (smallest == index)
			break ;
		heap_swap(&heap->entries[index], &heap->entries[smallest]);
		index = smallest;
	}
}
