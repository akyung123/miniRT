/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rander.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/13 23:58:56 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

//color 함수
// t_color를 받아서 int를 반환하도로 수정해야함
int	create_argb(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(color.x * 255.0);
	g = (int)(color.y * 255.0);
	b = (int)(color.z * 255.0);

	return ((r << 16) | (g << 8) | b);
}

void    my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
    char    *dst;

    // 색칠할 점이 이미지 크기 안에 있는지 확인하기
    if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
    {
        dst = mlx->addr + (y * mlx->size_line + x * (mlx->bpp / 8));
        *(unsigned int *)dst = color;
    }
}

t_color	color_lerp(t_color a, t_color b, double t)
{
	t_color	out;
	out.x = a.x * (1.0 - t) + b.x * t;
	out.y = a.y * (1.0 - t) + b.y * t;
	out.z = a.z * (1.0 - t) + b.z * t;
	return (out);
}

// 픽셀 단위로 렌더 함수
t_color	render_pixel(t_scene *scene, int x, int y)
{
    t_color c;
    double brightness;

    (void)x;
    (void)y;
    (void)scene;

    c.x = 0.0;
    c.y = 0.5;
    c.z = 0.5;
    // x값에 따라서 파랑색상 조절(rgb에서 b)
    //c.y = (double)x / (double)(WIN_WIDTH - 1);
    //c.z = (double)x / (double)(WIN_WIDTH - 1);

    // 은은한 빛은
    t_color al;
    al.x = 1.0;
    al.y = 1.0;
    al.z = 1.0;
    // 세기는 20%
    al.x *= 0.2;
    al.y *= 0.2;
    al.z *= 0.2;

    // if 조명이 파란색이라면? 조명이 scene안에 들어있음
    // 0.0 0.0 1.0이면 파란색임
    t_color l;
    l.x = 0.0;
    l.y = 0.5;
    l.z = 0.5;

    // 조명색 조정
    c.x = c.x * l.x + c.x * al.x;
    c.y = c.y * l.y + c.y * al.y;
    c.z = c.z * l.z + c.z * al.z;

    // 밝기 조절
    brightness = 0.5;
	c.x = c.x * brightness;
	c.y = c.y * brightness;
	c.z = c.z * brightness;

    return(c);
}

// t_color	render_pixel(t_scene *scene, int x, int y)
// {
// 	t_color	white;
// 	t_color	sky;
// 	double	t;
// 	(void)scene;
// 	(void)x;

// 	white.x = 1.0;
// 	white.y = 1.0;
// 	white.z = 1.0;

// 	sky.x = 0.5;
// 	sky.y = 0.7;
// 	sky.z = 1.0;

// 	t = (double)y / (double)(WIN_HEIGHT - 1);
// 	return (color_lerp(white, sky, t));
// }

void rander(t_minirt *mini)
{
    (void)mini;
    unsigned int    color;

    int x, y;
    x = 0;
    y = 0;
    while (x < WIN_WIDTH)
    {
        y = 0;
        while(y < WIN_HEIGHT)
        {
            color = mlx_get_color_value(mini->mlx->mlx_ptr, create_argb(render_pixel(mini->scene, x, y)));
            my_mlx_pixel_put(mini->mlx, x, y, color);
            y++;
        }
        x++;
    }
}

// 구성을 어떻게 하지?
// 렌더->렌더 픽셀(color를 반환함)->색상 변환->점 찍기