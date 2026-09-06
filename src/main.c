/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:13:39 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/06 15:06:13 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	is_numeric(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i] && i < 8)
	{
		if (argv[i][0] == '\0')
			return (1);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_args(char **argv, t_args *argz)
{
	argz->nb_coders = atoi(argv[1]);
	if (argz->nb_coders <= 0)
		return (1);
	argz->time_to_burnout = atoi(argv[2]);
	if (argz->time_to_burnout <= 0)
		return (1);
	argz->time_to_compile = atoi(argv[3]);
	if (argz->time_to_compile <= 0)
		return (1);
	argz->time_to_debug = atoi(argv[4]);
	if (argz->time_to_debug <= 0)
		return (1);
	argz->time_to_refactor = atoi(argv[5]);
	if (argz->time_to_refactor <= 0)
		return (1);
	argz->nb_compiles = atoi(argv[6]);
	if (argz->nb_compiles <= 0)
		return (1);
	argz->dongle_cooldown = atoi(argv[7]);
	if (argz->dongle_cooldown <= 0)
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		argz->scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		argz->scheduler = EDF;
	else
		return(1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_args argz;
	t_data data;
	t_heap heap_data;

	if (argc != 9)
	{
		write(2, "Incorrect number of arguments provided", 39);
		return (1);
	}

	if (is_numeric(argv))
	{
		printf("Error processing args");
		return (1);
	}

	if (validate_args(argv, &argz))
	{
		printf("Error processing args");
		return (1);
	}

	if (init_data(&data, &argz))
		return (1);


	//printf("The arguments are as follows:");
	//print_args(argz);
	print_args(data.args);
	print_coders(&data);
	destroy_heap(&heap_data);
	destroy_data(&data);
	return (0);
}