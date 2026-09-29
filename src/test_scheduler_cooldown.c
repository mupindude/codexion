/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_scheduler_cooldown.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 07:41:41 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/29 07:42:40 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(void)
{
	t_data		data;
	t_coder		coder1;
	t_coder		coder2;
	t_dongle	dongles[2];
	t_request	request1;
	t_request	request2;
	long		now;

	dongles[0].id = 0;
	dongles[1].id = 1;

	dongles[0].in_use = 0;
	dongles[1].in_use = 0;

	coder1.id = 1;
	coder1.left_dongle = &dongles[0];
	coder1.right_dongle = &dongles[1];

	coder2.id = 2;
	coder2.left_dongle = &dongles[0];
	coder2.right_dongle = &dongles[1];

	request1.s_coder = &coder1;
	request2.s_coder = &coder2;

	now = get_time_ms();

	/*
	 * Simulate coder 1 releasing the dongles.
	 * They cannot be used for another 200 ms.
	 */
	dongles[0].available_at = now + 200;
	dongles[1].available_at = now + 200;

	printf("Cooldown ends at: %ld\n", dongles[0].available_at);

	printf("\nCoder 2 tries immediately:\n");
	if (try_reserve_dongles(&request2, get_time_ms()))
		printf("ERROR: Coder 2 got the dongles during cooldown\n");
	else
		printf("Correct: Coder 2 must wait\n");

	usleep(250000);

	printf("\nCoder 2 tries after 250 ms:\n");
	if (try_reserve_dongles(&request2, get_time_ms()))
		printf("Correct: Coder 2 got the dongles\n");
	else
		printf("ERROR: Coder 2 was still blocked\n");

	return (0);
}