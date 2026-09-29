/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 07:32:57 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/29 07:33:29 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(void)
{
	t_coder		coder;
	t_request	*request;

	coder.id = 1;
	coder.last_compile = 1000;

	request = create_request(&coder, 1200, 500);
	if (!request)
		return (1);

	printf("Arrival time: %ld\n", request->arrival_time);
	printf("Last compile: %ld\n", coder.last_compile);
	printf("Deadline: %ld\n", request->deadline);

	free(request);
	return (0);
}