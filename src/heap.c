/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:43:30 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/06 14:48:58 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	compare_fifo(t_request *a, t_request *b)
{
	if (a->arrival_time < b->arrival_time)
		return -1;
	if (a->arrival_time > b->arrival_time)
		return (1);
	if (a->coder->id < b->coder->id)
		return (-1);
	if (a->coder->id > b->coder->id)
		return (1);
	return (0);
}

int	compare_edf(t_request *a, t_request *b)
{
	if (a->deadline < b->deadline)
		return (-1);
	if (a->deadline > b->deadline)
		return (1);
	if (a->coder->id < b->coder->id)
		return (-1);
	if (a->coder->id > b->coder->id)
		return (1);
	return (0);
}

int	heap_init(t_heap *heap, int capacity,
		int (*compare)(t_request *a, t_request *b))
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
		if (heap->compare(heap->entries[index],
				heap->entries[parent]) >= 0)
			break;
		heap_swap(&heap->entries[index],
			&heap->entries[parent]);
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
	int			index;
	int			left;
	int			right;
	int			smallest;

	if (heap->size == 0)
		return (NULL);

	result = heap->entries[0];
	heap->size--;

	if (heap->size == 0)
		return (result);

	heap->entries[0] = heap->entries[heap->size];
	index = 0;

	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		smallest = index;

		if (left < heap->size
			&& heap->compare(heap->entries[left],
				heap->entries[smallest]) < 0)
			smallest = left;

		if (right < heap->size
			&& heap->compare(heap->entries[right],
				heap->entries[smallest]) < 0)
			smallest = right;

		if (smallest == index)
			break;

		heap_swap(&heap->entries[index],
			&heap->entries[smallest]);
		index = smallest;
	}
	return (result);
}

void	destroy_heap(t_heap *heap)
{
	if ( heap == NULL)
		return ;
	free(heap->entries);
	heap->entries = NULL;
	heap->size = 0;
	heap->capacity = 0;
	heap->compare = NULL;
}