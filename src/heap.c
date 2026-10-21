/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:43:30 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 07:57:19 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	heap_init(t_heap *heap, int capacity, int (*compare)(t_request *a,
			t_request *b))
{
	heap->entries = malloc(sizeof(t_request *) * capacity);
	if (!heap->entries)
		return (1);
	heap->size = 0;
	heap->capacity = capacity;
	heap->compare = compare;
	return (0);
}

static void	heap_swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	heap_push(t_heap *heap, t_request *request)
{
	int	index;
	int	parent;

	if (heap->size >= heap->capacity)
		return (1);
	index = heap->size;
	heap->entries[index] = request;
	heap->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->compare(heap->entries[index], heap->entries[parent]) >= 0)
			break ;
		heap_swap(&heap->entries[index], &heap->entries[parent]);
		index = parent;
	}
	return (0);
}

t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->entries[0]);
}

t_request	*heap_pop(t_heap *heap)
{
	t_request	*result;

	if (heap->size == 0)
		return (NULL);
	result = heap->entries[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->entries[0] = heap->entries[heap->size];
		heap_sift_down(heap, 0);
	}
	return (result);
}
