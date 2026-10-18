/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heap_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 09:13:41 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/18 10:48:18 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int run_test(const char *name,
	int (*compare)(t_request *, t_request *))
{
	t_coder		coders[5];
	t_request	requests[5];
	t_heap		heap;
	t_request	*removed;
	t_request	*current;
	int			expected[4];
	int			i;
	int			failed;

	failed = 0;
	i = 0;
	while (i < 5)
	{
		coders[i].id = i + 1;
		requests[i].s_coder = &coders[i];
		requests[i].arrival_time = (long)((5 - i) * 10);
		requests[i].deadline = (long)((i + 1) * 100);
		requests[i].granted = 0;
		i++;
	}
	if (heap_init(&heap, 5, compare) != 0)
	{
		printf("%s: heap_init failed\n", name);
		return (1);
	}
	i = 0;
	while (i < 5)
	{
		if (heap_push(&heap, &requests[i]) != 0)
		{
			printf("%s: heap_push failed\n", name);
			destroy_heap(&heap);
			return (1);
		}
		i++;
	}
	removed = heap_remove_at(&heap, 2);
	if (removed != NULL)
    printf("%s: removed Coder %d\n", name, removed->s_coder->id);
	if (compare == compare_fifo && removed != &requests[1])
	{
		printf("%s: middle removal returned wrong request\n", name);
		failed = 1;
	}
	else if (compare == compare_edf && removed != &requests[2])
	{
		printf("%s: middle removal returned wrong request\n", name);
		failed = 1;
	}
	if (compare == compare_fifo)
	{
		expected[0] = 5;
		expected[1] = 4;
		expected[2] = 3;
		expected[3] = 1;
	}
	else
	{
		expected[0] = 1;
		expected[1] = 2;
		expected[2] = 4;
		expected[3] = 5;
	}
	i = 0;
	while (i < 4)
	{
		current = heap_pop(&heap);
		if (current == NULL)
		{
			printf("%s: unexpected empty heap\n", name);
			failed = 1;
			break ;
		}
		if (compare(current, &requests[expected[i] - 1]) != 0)
		{
			printf("%s: incorrect priority order at position %d\n",
				name, i);
			failed = 1;
		}
		i++;
	}
	if (heap_pop(&heap) != NULL)
	{
		printf("%s: heap should be empty\n", name);
		failed = 1;
	}
	destroy_heap(&heap);
	if (failed)
		printf("%s: FAIL\n", name);
	else
		printf("%s: PASS\n", name);
	return (failed);
}

static int test_root_removal(const char *name,
	int (*compare)(t_request *, t_request *))
{
	t_coder		coders[5];
	t_request	requests[5];
	t_heap		heap;
	t_request	*removed;
	t_request	*current;
	t_request	*previous;
	int			failed;
	int			i;

	failed = 0;
	i = 0;
	while (i < 5)
	{
		coders[i].id = i + 1;
		requests[i].s_coder = &coders[i];
		requests[i].arrival_time = (long)((5 - i) * 10);
		requests[i].deadline = (long)((i + 1) * 100);
		requests[i].granted = 0;
		i++;
	}
	if (heap_init(&heap, 5, compare) != 0)
		return (1);
	i = 0;
	while (i < 5)
	{
		if (heap_push(&heap, &requests[i]) != 0)
		{
			destroy_heap(&heap);
			return (1);
		}
		i++;
	}
	removed = heap_remove_at(&heap, 0);
	if (removed == NULL)
		failed = 1;
	previous = NULL;
	while ((current = heap_pop(&heap)) != NULL)
	{
		if (previous != NULL && compare(previous, current) > 0)
		{
			printf("%s: root removal broke heap order\n", name);
			failed = 1;
		}
		previous = current;
	}
	destroy_heap(&heap);
	if (failed)
		printf("%s root removal: FAIL\n", name);
	else
		printf("%s root removal: PASS\n", name);
	return (failed);
}

static int test_last_removal(const char *name,
	int (*compare)(t_request *, t_request *))
{
	t_coder		coders[5];
	t_request	requests[5];
	t_heap		heap;
	t_request	*removed;
	t_request	*current;
	t_request	*previous;
	int			failed;
	int			i;

	failed = 0;
	i = 0;
	while (i < 5)
	{
		coders[i].id = i + 1;
		requests[i].s_coder = &coders[i];
		requests[i].arrival_time = (long)((5 - i) * 10);
		requests[i].deadline = (long)((i + 1) * 100);
		requests[i].granted = 0;
		i++;
	}
	if (heap_init(&heap, 5, compare) != 0)
		return (1);
	i = 0;
	while (i < 5)
	{
		if (heap_push(&heap, &requests[i]) != 0)
		{
			destroy_heap(&heap);
			return (1);
		}
		i++;
	}
	removed = heap_remove_at(&heap, heap.size - 1);
	if (removed == NULL || heap.size != 4)
		failed = 1;
	previous = NULL;
	while ((current = heap_pop(&heap)) != NULL)
	{
		if (previous != NULL && compare(previous, current) > 0)
		{
			printf("%s: last removal broke heap order\n", name);
			failed = 1;
		}
		previous = current;
	}
	destroy_heap(&heap);
	if (failed)
		printf("%s last removal: FAIL\n", name);
	else
		printf("%s last removal: PASS\n", name);
	return (failed);
}

static int test_single_element_removal(
    const char *name,
    int (*compare)(t_request *, t_request *))
{
    t_coder coder;
    t_request request;
    t_heap heap;
    t_request *removed;
    int failed;

    coder.id = 1;
    request.s_coder = &coder;
    request.arrival_time = 10;
    request.deadline = 100;
    request.granted = 0;
    failed = 0;

    if (heap_init(&heap, 1, compare) != 0)
        return (1);
    if (heap_push(&heap, &request) != 0)
    {
        destroy_heap(&heap);
        return (1);
    }

    removed = heap_remove_at(&heap, 0);
    if (removed != &request || heap.size != 0)
        failed = 1;
    if (heap_pop(&heap) != NULL)
        failed = 1;

    destroy_heap(&heap);
    if (failed)
        printf("%s single-element removal: FAIL\n", name);
    else
        printf("%s single-element removal: PASS\n", name);
    return (failed);
}


static int test_invalid_removal(
    const char *name,
    int (*compare)(t_request *, t_request *))
{
    t_coder coders[3];
    t_request requests[3];
    t_heap heap;
    t_request *removed;
    int failed;
    int i;

    failed = 0;
    i = 0;
    while (i < 3)
    {
        coders[i].id = i + 1;
        requests[i].s_coder = &coders[i];
        requests[i].arrival_time = (long)((i + 1) * 10);
        requests[i].deadline = (long)((i + 1) * 100);
        requests[i].granted = 0;
        i++;
    }
    if (heap_init(&heap, 3, compare) != 0)
        return (1);
    i = 0;
    while (i < 3)
    {
        if (heap_push(&heap, &requests[i]) != 0)
        {
            destroy_heap(&heap);
            return (1);
        }
        i++;
    }

    removed = heap_remove_at(&heap, -1);
    if (removed != NULL || heap.size != 3)
        failed = 1;

    removed = heap_remove_at(&heap, heap.size);
    if (removed != NULL || heap.size != 3)
        failed = 1;

    i = 0;
    while (i < 3)
    {
        removed = heap_pop(&heap);
        if (removed == NULL || removed->s_coder->id != i + 1)
            failed = 1;
        i++;
    }
    if (heap_pop(&heap) != NULL)
        failed = 1;

    destroy_heap(&heap);
    if (failed)
        printf("%s invalid removal: FAIL\n", name);
    else
        printf("%s invalid removal: PASS\n", name);
    return (failed);
}


int	main(void)
{
	int	failed;

	failed = 0;
	failed += run_test("FIFO", compare_fifo);
	failed += run_test("EDF", compare_edf);

	failed += test_root_removal("FIFO", compare_fifo);
	failed += test_root_removal("EDF", compare_edf);

	failed += test_last_removal("FIFO", compare_fifo);
	failed += test_last_removal("EDF", compare_edf);

	failed += test_single_element_removal("FIFO", compare_fifo);
	failed += test_single_element_removal("EDF", compare_edf);

	failed += test_invalid_removal("FIFO", compare_fifo);
	failed += test_invalid_removal("EDF", compare_edf);
	return (failed != 0);
}