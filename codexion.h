#ifndef CODEXION_H
#define CODEXION_H

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_data		t_data;
typedef struct s_coder		t_coder;
typedef struct s_dongle		t_dongle;

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

	int			compile_count;
	long		last_compile;

	t_dongle	*left_dongle;
	t_dongle	*right_dongle;

	t_data		*data;
} t_coder;

typedef struct s_dongle
{
	int	id;
	int	in_use;
	long	cool_down;
	long	available_at;
	pthread_mutex_t	mutex;
} t_dongle;

int	is_numeric(char **argv);
int validate_args(char **argv, t_args *argz);
void print_args(t_args args);

int init_data(t_data *data, t_args *args);
/*
void	init_dongles(t_data data);
void	init_coders(t_data *data);
*/

#endif