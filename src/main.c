/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:13:39 by dmupindu          #+#    #+#             */
/*   Updated: 2026/09/20 14:42:19 by dmupindu         ###   ########.fr       */
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
	t_args	argz;
	t_data	data;
	int		i;

	if (argc != 9)
	{
		write(2, "Incorrect number of arguments provided", 39);
		return (1);
	}
	if (is_numeric(argv))
	{
		printf("Error processing args\n");
		return (1);
	}
	if (validate_args(argv, &argz))
	{
		printf("Error processing args\n");
		return (1);
	}
	if (init_data(&data, &argz))
		return (1);
	if (pthread_create(&data.scheduler, NULL,
			scheduler_routine, &data) != 0)
	{
		destroy_data(&data);
		return (1);
	}
	print_args(data.args);
	print_coders(&data);
	i = 0;
	while (i < data.args.nb_coders)
	{
		if (pthread_create(&data.coders[i].thread, NULL,
				coder_routine, &data.coders[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < data.args.nb_coders)
	{
		pthread_join(data.coders[i].thread, NULL);
		i++;
	}
	pthread_mutex_lock(&data.scheduler_mutex);
	data.stop = 1;
	pthread_cond_signal(&data.scheduler_cond);
	pthread_mutex_unlock(&data.scheduler_mutex);
	pthread_join(data.scheduler, NULL);
	destroy_data(&data);
	return (0);
}