/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_arrays.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 13:50:28 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/21 13:53:51 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_arrays(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * data->args.nb_coders);
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * data->args.nb_coders);
	if (!data->dongles)
	{
		free(data->coders);
		data->coders = NULL;
		return (1);
	}
	return (0);
}
