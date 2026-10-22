/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:13:39 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 08:39:21 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	check_arg_count(int argc)
{
	if (argc != 9)
	{
		fprintf(stderr,
			"Error: Expected 8 arguments: "
			"nb_coders time_to_burnout time_to_compile "
			"time_to_debug time_to_refactor nb_compiles "
			"dongle_cooldown fifo|edf\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_args	argz;
	t_data	data;
	int		created_coders;
	int		thread_error;

	if (check_arg_count(argc))
		return (1);
	if (is_numeric(argv))
		return (1);
	if (validate_args(argv, &argz))
		return (1);
	if (init_data(&data, &argz))
		return (1);
	if (start_simulation(&data))
		return (1);
	thread_error = start_coders(&data, &created_coders);
	join_threads(&data, created_coders);
	finish_simulation(&data);
	if (thread_error)
		return (1);
	return (0);
}
