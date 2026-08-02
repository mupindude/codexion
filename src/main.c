#include "../codexion.h"

int	is_numeric(char **argv)
{
	int	i;
	int j;

	i = 1;
	while (argv[i])
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

int validate_args(char **argv, t_args *argz)
{
	printf("Validating");
	argz->nb_coders = atoi(argv[1]);
	if (argz->nb_coders <= 0)
		return(1);
	argz->time_to_burnout = atoi(argv[2]);
	if (argz->time_to_burnout <= 0)
		return (1);
	argz->time_to_compile = atoi(argv[3]);
	if (argz->time_to_compile <= 0)
		return(1);
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
	argz->scheduler = atoi(argv[8]);
	if (argz->scheduler <= 0)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_args argz;

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

	printf("The arguments are as follows:");
	print_args(argz);
	return (0);
}