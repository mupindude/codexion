/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cooldown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 07:37:20 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/29 07:37:35 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(void)
{
	t_data		data;
	t_coder		coder;
	t_dongle	left;
	t_dongle	right;
	t_request	request;
	long		now;

	left.in_use = 0;
	right.in_use = 0;

	coder.left_dongle = &left;
	coder.right_dongle = &right;

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

	return (0);
}