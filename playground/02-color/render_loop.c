#include "mlx.h"
#include "output.h"

/* 본체 srcs/output/render_loop.c 와 같은 자리. 여기는 2단계 내내 안 건드린다.
 * 화면 내용을 바꾸고 싶으면 render_pixel 하나만 갈아끼운다.
 */

static void	put_pixel(t_mlx *mlx, int x, int y, int color)
{
	char	*dst;

	dst = mlx->addr + (y * mlx->size_line + x * (mlx->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

/* 2단계 체크리스트 3~7번은 전부 이 함수 본문만 바꾸는 것이다.
 *
 *   3. 단색       : c.x = 1.0; c.y = 0.0; c.z = 0.0;
 *   4. 좌우       : c.x = (double)x / (double)(WIN_WIDTH - 1);
 *   5. 2축        : 아래 (지금 이것)
 *   6. lerp       : t = (double)y / (double)(WIN_HEIGHT - 1);
 *                   c = color_lerp(white, sky, t);
 *   7. 클램프 시험: c.x = 1.5; c.y = -0.3; c.z = 0.5;  -> 0xFF007F
 *
 * WIDTH 가 아니라 WIDTH - 1 로 나눈다. 안 그러면 마지막 픽셀이
 * 799 / 800 = 0.99875 라서 끝이 정확히 1.0 에 닿지 않는다.
 */
t_color	render_pixel(t_scene *scene, int x, int y)
{
	t_color	c;

	(void)scene;
	c.x = (double)x / (double)(WIN_WIDTH - 1);
	c.y = (double)y / (double)(WIN_HEIGHT - 1);
	c.z = 0.5;
	return (c);
}

void	render_all(t_minirt *rt)
{
	int		x;
	int		y;
	t_color	color;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			color = render_pixel(rt->scene, x, y);
			put_pixel(&rt->mlx, x, y, color_to_int(color));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr,
		rt->mlx.img_ptr, 0, 0);
}
