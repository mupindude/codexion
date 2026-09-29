/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_edf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 07:13:34 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/29 07:20:35 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(void)
{
	t_request	a;
	t_request	b;
	t_request	c;
	t_request	*peek;

	a.s_coder = malloc(sizeof(t_coder));
	b.s_coder = malloc(sizeof(t_coder));
	c.s_coder = malloc(sizeof(t_coder));
	if (!a.s_coder || !b.s_coder || !c.s_coder)
		return (1);

	a.s_coder->id = 1;
	b.s_coder->id = 2;
	c.s_coder->id = 3;

	a.arrival_time = 10;
	a.deadline = 500;

	b.arrival_time = 20;
	b.deadline = 100;

	c.arrival_time = 30;
	c.deadline = 100;

	t_heap heap;

	if (heap_init(&heap, 3, compare_edf))
		return (1);

	heap_push(&heap, &a);
	heap_push(&heap, &b);
	heap_push(&heap, &c);

	printf("EDF PEEK:\n");
	peek = heap_peek(&heap);
	printf("Coder %d | arrival: %ld | deadline: %ld\n",
		peek->s_coder->id, peek->arrival_time, peek->deadline);

	printf("\nEDF POP ORDER:\n");
	while (heap.size > 0)
	{
		peek = heap_pop(&heap);
		printf("Coder %d | arrival: %ld | deadline: %ld\n",
			peek->s_coder->id, peek->arrival_time, peek->deadline);
	}

	destroy_heap(&heap);
	free(a.s_coder);
	free(b.s_coder);
	free(c.s_coder);
	return (0);
}