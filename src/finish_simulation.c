/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_simulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 08:04:43 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 08:09:52 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	finish_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.nb_coders)
	{
		printf("Coder %d compiled %d times\n",
			data->coders[i].id,
			data->coders[i].compile_count);
		i++;
	}
	destroy_data(data);
}
