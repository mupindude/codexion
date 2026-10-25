/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cooldown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 07:37:20 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/25 12:08:04 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(void)
{
	t_coder coder;
	t_dongle left;
	t_dongle right;
	t_request request;
	long now;

	if (pthread_mutex_init(&left.mutex, NULL) != 0)
    return (1);
	if (pthread_mutex_init(&right.mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&left.mutex);
		return (1);
	}

	left.in_use = 0;
	right.in_use = 0;

	coder.left_dongle = &left;
	coder.right_dongle = &right;

	left.id = 0;
	right.id = 1;
	coder.id = 1;

	request.s_coder = &coder;

	now = 1000;

	left.available_at = 1050;
	right.available_at = 1050;

	printf("Before cooldown expires:\n");
	if (try_reserve_dongles(&request, now))
		printf("ERROR: dongles were reserved too early\n");
	else
		printf("Correct: dongles are still cooling down\n");

	now = 1050;

	printf("\nAfter cooldown expires:\n");
	if (try_reserve_dongles(&request, now))
		printf("Correct: dongles were reserved\n");
	else
		printf("ERROR: dongles were not reserved\n");

	printf("\nTrying to reserve the same dongles again:\n");
	if (try_reserve_dongles(&request, now))
		printf("ERROR: dongles were reserved twice\n");
	else
		printf("Correct: dongles are already in use\n");

	pthread_mutex_destroy(&left.mutex);
	pthread_mutex_destroy(&right.mutex);
	return (0);
}