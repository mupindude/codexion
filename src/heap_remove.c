/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 07:55:42 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/25 16:52:32 by dmupindu         ###   ########.fr       */
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

static void	heap_sift_up(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->compare(heap->entries[index],
				heap->entries[parent]) >= 0)
			break ;
		heap_swap(&heap->entries[index], &heap->entries[parent]);
		index = parent;
	}
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

t_request	*heap_remove_at(t_heap *heap, int index)
{
	t_request	*result;

	if (heap == NULL || index < 0 || index >= heap->size)
		return (NULL);
	result = heap->entries[index];
	heap->size--;
	if (index == heap->size)
		return (result);
	heap->entries[index] = heap->entries[heap->size];
	if (index > 0 && heap->compare(heap->entries[index],
			heap->entries[(index - 1) / 2]) < 0)
		heap_sift_up(heap, index);
	else
		heap_sift_down(heap, index);
	return (result);
}

void	destroy_heap(t_heap *heap)
{
	if (heap == NULL)
		return ;
	free(heap->entries);
	heap->entries = NULL;
	heap->size = 0;
	heap->capacity = 0;
	heap->compare = NULL;
}
