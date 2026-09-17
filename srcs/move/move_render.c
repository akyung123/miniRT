/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "mlx.h"
#include "output.h"
#include "move.h"

static int	should_tick(void)
{
	static struct timeval	last;
	struct timeval			now;
	long					ms;

	gettimeofday(&now, NULL);
	ms = (now.tv_sec - last.tv_sec) * 1000L
		+ (now.tv_usec - last.tv_usec) / 1000L;
	if (last.tv_sec != 0 && ms < 16)
		return (0);
	last = now;
	return (1);
}

static void	put_px(t_mlx *mlx, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	dst = mlx->addr + (y * mlx->size_line + x * (mlx->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

static void	render_block(t_minirt *rt, int bx, int by, int size)
{
	int	color;
	int	sx;
	int	sy;
	int	x;
	int	y;

	sx = bx + size / 2;
	sy = by + size / 2;
	if (sx >= WIN_WIDTH)
		sx = WIN_WIDTH - 1;
	if (sy >= WIN_HEIGHT)
		sy = WIN_HEIGHT - 1;
	color = color_to_int(render_pixel(&rt->scene, sx, sy));
	y = by;
	while (y < by + size)
	{
		x = bx;
		while (x < bx + size)
		{
			put_px(&rt->mlx, x, y, color);
			x++;
		}
		y++;
	}
}

static void	render_preview(t_minirt *rt)
{
	int	x;
	int	y;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			render_block(rt, x, y, PREVIEW);
			x += PREVIEW;
		}
		y += PREVIEW;
	}
	mlx_put_image_to_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr,
		rt->mlx.img_ptr, 0, 0);
}

int	move_loop_hook(void *param)
{
	t_minirt	*rt;
	t_move		*st;

	if (!should_tick())
		return (0);
	rt = (t_minirt *)param;
	st = move_state();
	if (move_apply(&rt->scene.camera, st->held))
	{
		render_preview(rt);
		st->dirty = 1;
	}
	else if (st->dirty)
	{
		render_all(rt);
		st->dirty = 0;
	}
	return (0);
}
