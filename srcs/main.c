/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahadj-ar <ahadj-ar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:26:25 by ahadj-ar          #+#    #+#             */
/*   Updated: 2024/11/14 13:29:21 by ahadj-ar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	main(int ac, char **av)
{
	t_cube	cube;
	t_data	data;

	if (ac != 2)
	{
		printf("Wrong arguments\n");
		return (1);
	}
	else
	{
		cube.data = &data;
		init_data(&data);
		if (parsing(av, &cube))
		{
			cleanup(&cube);
			return (1);
		}
	}
	cleanup(&cube);
	return (0);
}
