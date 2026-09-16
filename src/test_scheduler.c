/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scheduler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:35:41 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/16 13:49:52 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	print_request(t_request *request)
{
	printf("Coder %d | arrival: %ld | deadline: %ld\n",
		request->s_coder->id,
		request->arrival_time,
		request->deadline);
}

int	main(void)
{
	t_data		data;
	t_coder		coders[4];
	int			i;

	data.args.nb_coders = 4;
	data.args.time_to_burnout = 1000;
	data.args.scheduler = FIFO;
	data.waiters = malloc(sizeof(t_heap));
	if (!data.waiters)
		return (1);

	if (heap_init(data.waiters, 4, compare_fifo))
	{
		free(data.waiters);
		return (1);
	}

	i = 0;
	while (i < 4)
	{
		coders[i].id = i + 1;
		coders[i].data = &data;
		i++;
	}

	submit_request(&data, &coders[0], 40);
	submit_request(&data, &coders[1], 10);
	submit_request(&data, &coders[2], 30);
	submit_request(&data, &coders[3], 20);

	printf("FIFO POP ORDER:\n");
	while (data.waiters->size > 0)
	{
		print_request(heap_pop(data.waiters));
	}

	destroy_heap(data.waiters);
	free(data.waiters);
	return (0);
}