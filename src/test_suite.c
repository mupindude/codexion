/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_suite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:23:19 by dmupindu          #+#    #+#             */
/*   Updated: 2026/08/28 08:51:57 by dmupindu         ###   ########.fr       */
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
	printf("scheduler        : %s\n", args.scheduler);
}