/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 15:14:13 by dmupindu          #+#    #+#             */
/*   Updated: 2026/08/28 08:36:20 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct s_args
{
	int					nb_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					nb_compiles;
	int					dongle_cooldown;
	t_scheduler			scheduler;
}						t_args;

typedef enum e_scheduler
{
	FIFO,
	EDF
} t_scheduler;

typedef struct s_data
{
	t_args				args;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor;

	long				start_time;
	int					stop;

	pthread_mutex_t		print_mutex;
	pthread_mutex_t		stop_mutex;

}						t_data;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;

	int					compile_count;
	long				last_compile;

	t_dongle			*left_dongle;
	t_dongle			*right_dongle;

	t_data				*data;
}						t_coder;

typedef struct s_dongle
{
	int					id;
	int					in_use;
	long				available_at;
	t_heap				waiters;
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
}						t_dongle;

typedef struct  s_request
{
	t_coder	*s_coder;
	long	arrival_time;
	long	deadline;
}	t_request;

typedef	struct s_heap
{
	t_request	**entries;
	int			size;
	int			capacity;
	int			(*compare) (t_request *a, t_request *b);
}	t_heap;



int						is_numeric(char **argv);
int						validate_args(char **argv, t_args *argz);
void					print_args(t_args args);

int						init_data(t_data *data, t_args *args);
/*
void					init_dongles(t_data data);
void					init_coders(t_data *data);
*/

#endif