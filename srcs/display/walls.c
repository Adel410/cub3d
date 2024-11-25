/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahadj-ar <ahadj-ar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:07:14 by ahadj-ar          #+#    #+#             */
/*   Updated: 2024/11/25 17:48:05 by ahadj-ar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	display_vertical_line(t_cube *cube, int x, int start, int end)
{
	int	y;
	int	color;

	(void)x;
	(void)end;
	color = 0x00696969;
	y = start;
	while (y <= end)
	{
		ft_pixel_put(cube->img, x, y, color);
		y++;
	}
}

void	walls(t_cube *cube, int x, double ray_len)
{
	t_wall	wall;

	wall.line_height = (int)(W_HEIGHT / ray_len);
	wall.draw_start = -wall.line_height / 2 + W_HEIGHT / 2;
	if (wall.draw_start < 0)
		wall.draw_start = 0;
	wall.draw_end = wall.line_height / 2 + W_HEIGHT / 2;
	if (wall.draw_end >= W_HEIGHT)
		wall.draw_end = W_HEIGHT - 1;
	display_vertical_line(cube, x, wall.draw_start, wall.draw_end);
}
