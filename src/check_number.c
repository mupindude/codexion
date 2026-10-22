/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmupindu <dmupindu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 07:52:53 by dmupindu          #+#    #+#             */
/*   Updated: 2026/10/22 07:53:52 by dmupindu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	report_number_error(int index, int error)
{
	if (error == 1)
		fprintf(stderr,
			"Error: Argument %d must be a positive integer.\n", index);
	else if (error == 2)
		fprintf(stderr,
			"Error: Argument %d must contain only digits.\n", index);
	else
		fprintf(stderr,
			"Error: Argument %d exceeds the maximum integer value.\n",
			index);
	return (1);
}

static int	check_number(char *arg, int index)
{
	int	j;
	int	digit;
	int	value;

	if (arg[0] == '\0')
		return (report_number_error(index, 1));
	j = 0;
	value = 0;
	while (arg[j])
	{
		if (arg[j] < '0' || arg[j] > '9')
			return (report_number_error(index, 2));
		digit = arg[j] - '0';
		if (value > (2147483647 - digit) / 10)
			return (report_number_error(index, 3));
		value = value * 10 + digit;
		j++;
	}
	return (0);
}

int	is_numeric(char **argv)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (check_number(argv[i], i))
			return (1);
		i++;
	}
	return (0);
}
