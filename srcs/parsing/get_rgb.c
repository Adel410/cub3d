/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_rgb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahadj-ar <ahadj-ar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:30:00 by ahadj-ar          #+#    #+#             */
/*   Updated: 2025/03/02 14:57:03 by ahadj-ar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	check_data(t_data *data)
{
	if (data->c_line && data->f_line && data->we_img && data->ea_img
		&& data->no_img && data->so_img)
		return (0);
	else
		return (1);
}

int	count_commas(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == ',')
			count++;
		i++;
	}
	return (count);
}

int	create_rgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int	convert_rgb(char **rgb)
{
	int	r;
	int	g;
	int	b;

	r = 0;
	g = 0;
	b = 0;
	if (ft_tablen(rgb) < 4 || ft_tablen(rgb) > 4)
		return (ft_putstr_fd("RGB values incomplete\n", 2), -1);
	if (!ft_is_valid_number(rgb[1]) || !ft_is_valid_number(rgb[2])
		|| !ft_is_valid_number(rgb[3]))
		return (ft_putstr_fd("Invalid RGB values\n", 2), -1);
	if (rgb[1])
		r = ft_atoi(rgb[1]);
	if (rgb[2])
		g = ft_atoi(rgb[2]);
	if (rgb[3])
		b = ft_atoi(rgb[3]);
	if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0)
		return (ft_putstr_fd("Invalid RGB values\n", 2), -1);
	return (create_rgb(0, r, g, b));
}

int	get_rgb(char *color)
{
	char	**split_tab;
	char	*line;
	int		i;

	split_tab = ft_split(color, ',');
	if (!split_tab || ft_tablen(split_tab) != 3 || count_commas(color) != 2)
		return (ft_putstr_fd("Invalid RGB values\n", 2), free_tab(split_tab),
			-1);
	i = -1;
	line = ft_calloc(1, 1);
	while (split_tab[++i])
	{
		line = ft_strjoin(line, split_tab[i]);
		line = ft_strjoin(line, " ");
	}
	free_tab(split_tab);
	split_tab = ft_split(line, ' ');
	free(line);
	if (!split_tab)
		return (-1);
	i = convert_rgb(split_tab);
	if (i == -1)
		return (free_tab(split_tab), -1);
	free_tab(split_tab);
	return (i);
}
