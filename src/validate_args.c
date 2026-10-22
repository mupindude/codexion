/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 07:39:14 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 07:53:37 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	set_positive_arg(char *value, int *target, char *message)
{
	*target = atoi(value);
	if (*target <= 0)
	{
		fprintf(stderr, "%s\n", message);
		return (1);
	}
	return (0);
}

static int	validate_numeric_args(char **argv, t_args *argz)
{
	argz->nb_coders = atoi(argv[1]);
	if (argz->nb_coders < 2)
	{
		fprintf(stderr, "Error: At least 2 coders are required.\n");
		return (1);
	}
	if (set_positive_arg(argv[2], &argz->time_to_burnout,
			"Error: Time to burnout must be greater than 0.")
		|| set_positive_arg(argv[3], &argz->time_to_compile,
			"Error: Time to compile must be greater than 0.")
		|| set_positive_arg(argv[4], &argz->time_to_debug,
			"Error: Time to debug must be greater than 0.")
		|| set_positive_arg(argv[5], &argz->time_to_refactor,
			"Error: Time to refactor must be greater than 0.")
		|| set_positive_arg(argv[6], &argz->nb_compiles,
			"Error: Number of compiles must be greater than 0.")
		|| set_positive_arg(argv[7], &argz->dongle_cooldown,
			"Error: Dongle cooldown must be greater than 0."))
		return (1);
	return (0);
}

int	validate_args(char **argv, t_args *argz)
{
	if (validate_numeric_args(argv, argz))
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		argz->scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		argz->scheduler = EDF;
	else
	{
		fprintf(stderr, "Error: Scheduler must be 'fifo' or 'edf'.\n");
		return (1);
	}
	return (0);
}
