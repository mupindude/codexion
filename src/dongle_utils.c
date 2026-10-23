/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 07:46:28 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/23 07:53:03 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	check_and_reserve(t_dongle *left, t_dongle *right, long now,
				int reserve);
static void	release_pair(t_dongle *left, t_dongle *right, long available_at);

static int	check_and_reserve(t_dongle *left, t_dongle *right, long now,
		int reserve)
{
	t_dongle	*first;
	t_dongle	*second;
	int			available;

	first = left;
	second = right;
	if (first->id > second->id)
	{
		first = right;
		second = left;
	}
	pthread_mutex_lock(&first->mutex);
	if (first != second)
		pthread_mutex_lock(&second->mutex);
	available = (left->in_use == 0 && left->available_at <= now
			&& right->in_use == 0 && right->available_at <= now);
	if (available && reserve)
	{
		left->in_use = 1;
		right->in_use = 1;
	}
	if (first != second)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (available);
}

int	try_reserve_dongles(t_request *request, long now)
{
	t_coder	*coder;

	coder = request->s_coder;
	return (check_and_reserve(coder->left_dongle, coder->right_dongle, now, 1));
}

int	dongles_are_available(t_request *request, long now)
{
	t_coder	*coder;

	coder = request->s_coder;
	return (check_and_reserve(coder->left_dongle, coder->right_dongle, now, 0));
}

static void	release_pair(t_dongle *left, t_dongle *right, long available_at)
{
	t_dongle	*first;
	t_dongle	*second;

	first = left;
	second = right;
	if (first->id > second->id)
	{
		first = right;
		second = left;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	left->in_use = 0;
	left->available_at = available_at;
	right->in_use = 0;
	right->available_at = available_at;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}

void	release_dongles(t_request *request)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;
	long		available_at;
	t_data		*data;

	coder = request->s_coder;
	data = coder->data;
	left = coder->left_dongle;
	right = coder->right_dongle;
	available_at = get_time_ms() + data->args.dongle_cooldown;
	if (left == right)
	{
		pthread_mutex_lock(&left->mutex);
		left->in_use = 0;
		left->available_at = available_at;
		pthread_mutex_unlock(&left->mutex);
	}
	else
		release_pair(left, right, available_at);
	pthread_mutex_lock(&data->scheduler_mutex);
	pthread_cond_broadcast(&data->scheduler_cond);
	pthread_mutex_unlock(&data->scheduler_mutex);
}
