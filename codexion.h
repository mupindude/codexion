#ifndef CODEXION_H
#define CODEXION_H

typedef struct s_args
{
	int	nb_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	nb_compiles;
	int	dongle_cooldown;
	int	scheduler;
}	t_args;

typedef struct s_data
{
	t_args		args;
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	monitor;

	long		start_time;
	int			stop;

	pthread_mutex_t	print_mutex;
    pthread_mutex_t	stop_mutex;

} t_data;

typedef struct s_coder
{
	int			id;
	pthread_t   thread;
	t_data		*data;
} t_coder;

typedef struct s_dongle
{
	int	id;
	int	in_use;
	long	cool_down;
	pthread_mutex_t	mutex;
} t_dongle;

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#endif