#include "../codexion.h"

int init_data(t_data *data, t_args *args)
{
	int	i;

	data->args = *args;
	data->stop = 0;
	data->start_time = 0;
	data->coders = malloc(sizeof(t_coder) * data->args.nb_coders);
	if (!data->coders)
		return (1);

	data->dongles = malloc(sizeof(t_dongle) * data->args.nb_coders);
	if (!data->dongles)
	{
		free(data->coders);
		return (1);
	}

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);

	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}

	while (i < data->args.nb_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].data = data;

		data->dongles[i].id = i;
		data->dongles[i].in_use = 0;
		data->dongles[i].cool_down = 0;

		i++;
	}

	return (0);
}

void	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].in_use = 0;
		data->dongles[i].available_at = 0;
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		i++;
	}
}