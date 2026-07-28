#include "../codexion.h"

int validate_arg(char *arg)
{
	int i;

	if (arg[0] == '\0')
		return (1);
	i = 0;

	while(arg[i])
	{
		if (arg[i] < '0' || arg[i]> '9')
			return (1);
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 9)
	{
		write(2, "Incorrect number of arguments provided", 39);
		return(1);
	}

	int i = 1;
	while (i < argc)
	{
		if (validate_arg(argv[i]))
		{
			write(2, "Invalid Arguments", 18);
			return (1);
		}
		i++;
	}
	return (0);
}