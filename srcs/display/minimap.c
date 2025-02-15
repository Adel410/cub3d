/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:59:37 by ahadj-ar          #+#    #+#             */
/*   Updated: 2025/02/15 20:22:12 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/** INIT_MINIMAP 
 * - linked to the game data
 * - Get actual map dimensions -> determine max width/length
 * - set scale (2 pixel per tile ensure visuality)
 * - set width/heigth-> big map x/y * scale
*/
void init_minimap(t_cube *cube)
{
    int max_dimension;

    if (!cube || !cube->data || !cube->data->map.map)
        return;

    cube->data->map.height = 0;
    while (cube->data->map.map[cube->data->map.height])
        cube->data->map.height++;

    max_dimension = cube->data->map.width;
    if (cube->data->map.height > cube->data->map.width)
        max_dimension = cube->data->map.height;

    cube->data->minimap.scale = MINIMAP_SIZE / max_dimension;
    if (cube->data->minimap.scale < 2)
        cube->data->minimap.scale = 2;

    cube->data->minimap.mn_width = cube->data->map.width * cube->data->minimap.scale;
    cube->data->minimap.mn_height = cube->data->map.height * cube->data->minimap.scale;

    cube->data->minimap.offset_x = 10;
    cube->data->minimap.offset_y = W_HEIGHT - cube->data->minimap.mn_height - 10;
}

/** INIT_MINIMAP_MLX
 * -Creating a blank image buffer where you can draw!!
 * - put mini_img img -> if fail, error msg
 * - get mini_img addr -> if fail, error msg
*/
int init_minimap_mlx(t_cube *cube)
{
    cube->data->mini_img.img =  mlx_new_image(cube->mlx, cube->data->minimap.mn_width, cube->data->minimap.mn_height);
    if (!cube->data->mini_img.img)
    {
        ft_putstr_fd("Minimap creation failed (img)\n", 2);
        return (1);
    }
    cube->data->mini_img.addr = mlx_get_data_addr(cube->data->mini_img.img, &cube->data->mini_img.bpp, &cube->data->mini_img.line_length, &cube->data->mini_img.endian);
    if (!cube->data->mini_img.addr)
    {
        ft_putstr_fd("Minimap creation failed (img addr)\n", 2);
        return (1);
    }    
    return (0);
}

/** SET_MINIMAP_CONTENT 
 * - set minimap content
 * - loop x/y -> put pixel color
 * - loop y then x -> top-bottom then left-right
 * - wall(1) , path(0)
*/
void    set_minimap_content(t_data *data)
{
    int x;
    int y;

    if (!data || !data->map.map)
        return;
    y = -1;
    while (++y < data->map.height)
    {
        x = -1;
        while (++x < data->map.width)
        {
            if (data->map.map[y][x] == '1')
                put_minimap_pixel(data, x, y, 0x2f6299); //blue 
            else if ((data->map.map[y][x] == '0' || data->map.map[y][x] == 'S' || data->map.map[y][x] == 'W' || data->map.map[y][x] == 'E' || data->map.map[y][x] == 'N'))
                put_minimap_pixel(data, x, y, 0xebb88f); //yellow
            else//background
                put_minimap_pixel(data, x, y, 0x002f6299); //blue//no transsparent :(
        }
    }
}

/** PUT_MINMAP_PIXEL
 * - put pixel color
 * @note Add bounds checking
 */
void put_minimap_pixel(t_data *data, int map_x, int map_y, int color)
{
    int i;
    int j;
    int screen_x;
    int screen_y;

    i = -1;
    while (++i < data->minimap.scale)
    {
        j = -1;
        while (++j < data->minimap.scale)
        {
            screen_x = map_x * data->minimap.scale + i;
            screen_y = map_y * data->minimap.scale + j;
            if (screen_x >= 0 && screen_x < data->minimap.mn_width &&
                screen_y >= 0 && screen_y < data->minimap.mn_height)
            {
                ft_pixel_put(&data->mini_img, screen_x, screen_y, color);
            }
        }
    }
}
/** SET PLAYER ON MINIMAP 
 * - get player pos
 * @note pos *scale !! 
 * -> convert world coordinates to minimap coordinates!!
 * @note draw little circle (radius 3)
 * @note Boundary check before drawing the pixel
*/
void    set_player_on_minimap(t_data *data)
{
    int player_x;
    int player_y;
    int x;
    int y;

    player_x = data->p_pos.x * data->minimap.scale;
    player_y = data->p_pos.y * data->minimap.scale;
    
    //draw circle
    y = -4;
    while (y <= 4)
    {
        x = -4;
        while (x <= 4)
        {
            if (x * x + y * y <= 16)
            {     
                if (player_x + x >= 0 && player_x + x < data->minimap.mn_width &&
                    player_y + y >= 0 && player_y + y < data->minimap.mn_height)
                ft_pixel_put(&data->mini_img, player_x + x, player_y + y, 0xFF0000);// Red
            }
            x++;
        }
        y++;
    }
}

/** MAIN MINIMAP
 * 
 * 
*/
int	minimap(t_cube *cube)
{
    if (!cube->data || !cube->data->map.map || !cube->data->mini_img.img)
    // if (!cube->data)// || other condition?
        return (1);

    // Make sure map dimensions are properly set
    // while (cube->data->map.map[cube->data->map.height])
    //     cube->data->map.height++;

    set_minimap_content(cube->data);
    set_player_on_minimap(cube->data);
    return (0);
}

/* CLEANUP MINIMAP
// Free the minimap image (main rendering image)
//texture (if have player info later)
//wall (if have player info later) //MAYBE NO NEED?
//img, just in case
*/
void	cleanup_minimap(t_cube *cube)
{
    //SHOULD PROB ONLY NEED THIS
    if (cube->data->mini_img.img != NULL)
    {
        mlx_destroy_image(cube->mlx, cube->data->mini_img.img);
        cube->data->mini_img.img = NULL;
    }
    // if (cube->data->mini_img.textures)
    // {
    //     free(cube->data->mini_img.textures);
    //     cube->data->mini_img.textures = NULL;
    // }
    // if (cube->data->mini_img.wall)
    // {
    //     free(cube->data->mini_img.wall);
    //     cube->data->mini_img.wall = NULL;
    // }   
}


//  OGGGG
// void	init_minimap(t_cube *cube)
// // void	init_minimap(t_data *data)
// {
//     int max_dimension;

//     max_dimension = cube->data->map.width;
//     if ( cube->data->map.height > cube->data->map.width)
//         max_dimension = cube->data->map.height;
//     //WRONGG: i need to get lengh of map!!try with new struc:
//     // max_dimension = data->map.x;
//     // if (data->map.y > data->map.x)
//     //     max_dimension = data->map.y;

//     cube->data->minimap.scale =  MINIMAP_SIZE / max_dimension;
//     if ( cube->data->minimap.scale < 2)//4?
//         cube->data->minimap.scale = 2;//4?
    
//     cube->data->minimap.mn_width = cube->data->map.width * cube->data->minimap.scale;
//     cube->data->minimap.mn_height = cube->data->map.height * cube->data->minimap.scale;
//     // cube->data->minimap.mn_width = 250;
//     // cube->data->minimap.mn_height = 150;

//     cube->data->minimap.offset_x = 10;
//     cube->data->minimap.offset_y = W_HEIGHT - cube->data->minimap.mn_height - 10;
//     // printf("👹cube->data->xpm_height: %d\n", cube->data->xpm_height);
//     // printf("👹cube->data->xpm_height: %d\n", cube->data->xpm_height);
//     // printf("👹data->xpm_height: %d\n", cube->data->xpm_height);
//     // printf("👹data->xpm_width;: %d\n", cube->data->xpm_width);
//     // printf("👹data->map.x: %d\n", cube->data->map.x);
//     // printf("👹data->map.y: %d\n", cube->data->map.y);
//     // printf("max_dimension: %d\n", max_dimension);
//     // printf("👹data->minimap.scale: %f\n", cube->data->minimap.scale);
// }