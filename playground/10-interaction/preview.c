/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preview.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// block x block 칸 하나를 한 색으로 칠한다 (화면 밖은 my_mlx_pixel_put 이 걸러준다)
static void	fill_block(t_mlx *mlx, int x0, int y0, int block, int color)
{
	int	x;
	int	y;

	y = y0;
	while (y < y0 + block)
	{
		x = x0;
		while (x < x0 + block)
		{
			my_mlx_pixel_put(mlx, x, y, color);
			x++;
		}
		y++;
	}
}

/*
저해상도 미리보기

800x600 = 48만 개 광선. 움직일 때마다 전부 쏘면 키를 누르고 한참 기다린다
block = 4 면 칸 16개에 광선 1개 → 계산량 1/16 (200x150 으로 렌더하고 4배 확대한 것과 같다)

  ┌───┬───┬───┐
  │ ● │ ● │ ● │   ● = 실제로 render_pixel 을 부르는 점 (칸의 가운데)
  ├───┼───┼───┤       칸 나머지 15픽셀은 같은 색으로 채운다
  │ ● │ ● │ ● │
  └───┴───┴───┘

render_pixel 은 원래 좌표 (x, y) 를 그대로 받으니 렌더러는 한 줄도 안 바뀐다
칸의 '가운데' 를 쓰는 이유: 왼쪽 위 모서리를 쓰면 그림 전체가 block/2 픽셀만큼 오른쪽 아래로 밀려 보인다
마지막 줄/열에서 가운데가 화면 밖으로 나가면 화면 끝 픽셀로 당긴다

block = 1 이면 rander() 와 같은 전체 해상도
이미지는 새로 만들지 않고 init 때 만든 하나를 계속 덮어쓴다 (매 프레임 mlx_new_image 는 메모리가 샌다)
*/
void	render_scaled(t_minirt *mini, int block)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		sy = y + block / 2;
		if (sy >= WIN_HEIGHT)
			sy = WIN_HEIGHT - 1;
		x = 0;
		while (x < WIN_WIDTH)
		{
			sx = x + block / 2;
			if (sx >= WIN_WIDTH)
				sx = WIN_WIDTH - 1;
			fill_block(&mini->mlx, x, y, block,
				create_argb(render_pixel(&mini->scene, sx, sy)));
			x += block;
		}
		y += block;
	}
}
