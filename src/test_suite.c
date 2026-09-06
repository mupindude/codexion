/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_suite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:23:19 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/01 13:37:57 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	print_args(t_args args)
{
	printf("nb_coders        : %d\n", args.nb_coders);
	printf("time_to_burnout  : %d\n", args.time_to_burnout);
	printf("time_to_compile  : %d\n", args.time_to_compile);
	printf("time_to_debug    : %d\n", args.time_to_debug);
	printf("time_to_refactor : %d\n", args.time_to_refactor);
	printf("nb_compiles      : %d\n", args.nb_compiles);
	printf("dongle_cooldown  : %d\n", args.dongle_cooldown);
	printf("scheduler        : %d\n", args.scheduler);
}

void	print_coders(t_data *data) //function to test the initialization
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		printf("Coder %d\n", data->coders[i].id);
		printf("  left  dongle: %d\n",
			data->coders[i].left_dongle->id);
		printf("  right dongle: %d\n",
			data->coders[i].right_dongle->id);
		printf("  compile count: %d\n",
			data->coders[i].compile_count);
		printf("  last compile: %ld\n",
			data->coders[i].last_compile);
		i++;
	}
}