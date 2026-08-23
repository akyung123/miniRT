/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/07/25 23:58:56 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	create_argb(int a, int r, int g, int b)
{
	return ((a << 24 | r << 16) | (g << 8) | b);
}

// 각 도형에 따른 그리기 추가
// 사각형과 원을 그리기
// 각 draw에는 위치, 색상이 주어진다
// 위치는 center 좌표이다.

void    my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
    char    *dst;

    // 색칠할 점이 이미지 크기 안에 있는지 확인하기
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        dst = mlx->addr + (y * mlx->size_line + x * (mlx->bpp / 8));
        *(unsigned int *)dst = color;
    }
}

// 중심에서부터의 거리를 측정해서 r이 되는 좌표들만 색칠한다
void    draw_circle(t_mlx *mlx, int x, int y, int r, int color)
{

    int i = x - r;
    int j = y - r;
    while (i <= x + r)
    {
        j = y - r;
        while (j <= y + r)
        {
            if (((i - x)*(i - x) + (j - y)*(j - y)) <  r*r)
                my_mlx_pixel_put(mlx, i, j, color);
            j++;
        }
        i++;
    }
}

void    draw_square(t_mlx *mlx, int x1, int x2, int y1, int y2, int color)
{
    int x;
    int y;
    x = x1;
    while (x1 <= x && x < x2)
    {
        y = y1;
        while (y1 <= y && y < y2)
        {
            my_mlx_pixel_put(mlx, x, y, color);
            y++;
        }
        x++;
    }
}

void	draw(t_mini *mini)
{
	(void)mini;
    int color;

	color = create_argb(0, 255, 255, 255);
    draw_circle(mini->mlx, WIDTH / 2, HEIGHT / 2, 100, color);
    draw_square(mini->mlx, 10, 40, 60, 100, color);
	// mlx_pixel_put은 창에 직접 점을 찍는다.
	// my_mlx_pixel_put(mini->mlx, WIDTH / 2, HEIGHT / 2, color);
}
