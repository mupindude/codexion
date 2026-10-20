/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phase.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 13:35:18 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/20 13:51:49 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	perform_debug(t_coder *coder)
{
	t_data	*data;

	data = coder->data;
	log_state(coder, "is debugging");
	usleep(data->args.time_to_debug * 1000);
}

void	perform_refactor(t_coder *coder)
{
	t_data	*data;

	data = coder->data;
	log_state(coder, "is refactoring");
	usleep(data->args.time_to_refactor * 1000);
}
