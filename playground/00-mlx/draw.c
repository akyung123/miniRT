/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 22:58:35 by akkim             #+#    #+#             */
/*   Updated: 2026/07/26 00:04:18 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	create_argb(int a, int r, int g, int b)
{
	return ((a << 24 | r << 16) | (g << 8) | b);
}

t_point	*point_create(double x, double y, double z)
{
	t_point	*result;

	result = (t_point*)malloc(sizeof(t_point));
	result->x = x;
	result->y = y;
	result->z = z;
	return (result);
}
double	clamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

int	get_color_val(t_point *color)
{
	int		x;
	int		y;
	int		z;

	x = clamp(color->x, 0.0, 0.999) * 256.0;
	y = clamp(color->y, 0.0, 0.999) * 256.0;
	z = clamp(color->z, 0.0, 0.999) * 256.0;
	return (x << 16 | y << 8 | z);
}

void	my_mlx_pixel_put(t_image *image, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		dst = image->addr + (y * image->size_line + x * (image->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

// draw면 어떤걸 그릴까요?
void draw_c2(t_mini *mini)
{
	int x, y;
	int color;

	color = create_argb(0, 255, 255, 255);

	y = HEIGHT / 2 - 50;
	while (y < HEIGHT / 2 + 50)
	{
		x = WIDTH / 2 - 50;
		while (x < WIDTH / 2 + 50)
		{
			my_mlx_pixel_put(mini->image, x, y, color);
			x++;
		}
		y++;
	}
}