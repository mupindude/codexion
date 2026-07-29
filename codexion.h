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
	t_coder	*coders;
	t_dongle	*dongles;
	pthread_t	monitoring;

} t_data;

typedef struct s_coder
{
	int	id;

} t_coder;

typedef struct s_dongle
{
	int	id;
	int	cool_down;
} t_dongle;

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#endif