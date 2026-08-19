#include "mlx.h"
#include "output.h"

static void	put_pixel(t_mlx *mlx, int x, int y, int color)
{
	char	*dst;

	dst = mlx->addr + (y * mlx->size_line + x * (mlx->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

int	color_to_int(t_color color)
{
	if (color.x < 0.0)
		color.x = 0.0;
	if (color.x > 1.0)
		color.x = 1.0;
	if (color.y < 0.0)
		color.y = 0.0;
	if (color.y > 1.0)
		color.y = 1.0;
	if (color.z < 0.0)
		color.z = 0.0;
	if (color.z > 1.0)
		color.z = 1.0;
	return (((int)(color.x * 255.0) << 16)
		| ((int)(color.y * 255.0) << 8)
		| (int)(color.z * 255.0));
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
			color = render_pixel(&rt->scene, x, y);
			put_pixel(&rt->mlx, x, y, color_to_int(color));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr,
		rt->mlx.img_ptr, 0, 0);
}
