#include "../codexion.h"

int	is_numeric(char *arg)
{
	int	i;

	if (arg[0] == '\0')
		return (1);
	i = 0;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int		i;
	t_args argz;
	if (argc != 9)
	{
		write(2, "Incorrect number of arguments provided", 39);
		return (1);
	}

	i = 1;
	while (i < argc)
	{
		if (is_numeric(argv[i]))
		{
			write(2, "Invalid Arguments", 18);
			return (1);
		}
		i++;
	}

	argz.nb_coders = atoi(argv[1]);
	argz.time_to_burnout = atoi(argv[2]);
	argz.time_to_compile = atoi(argv[3]);
	argz.time_to_debug = atoi(argv[4]);
	argz.time_to_refactor = atoi(argv[5]);
	argz.nb_compiles = atoi(argv[6]);
	argz.dongle_cooldown = atoi(argv[7]);
	argz.scheduler = atoi(argv[8]);
	return (0);
}