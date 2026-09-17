#include <stdlib.h>
#include "mlx.h"
#include "output.h"

#ifndef __APPLE__

static void	destroy_display(void *mlx_ptr)
{
	mlx_destroy_display(mlx_ptr);
	free(mlx_ptr);
}

#else

static void	destroy_display(void *mlx_ptr)
{
	(void)mlx_ptr;
}

#endif

void	cleanup_mlx(t_minirt *rt)
{
	if (!rt->mlx.mlx_ptr)
		return ;
	if (rt->mlx.img_ptr)
		mlx_destroy_image(rt->mlx.mlx_ptr, rt->mlx.img_ptr);
	if (rt->mlx.win_ptr)
		mlx_destroy_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr);
	rt->mlx.img_ptr = NULL;
	rt->mlx.win_ptr = NULL;
	destroy_display(rt->mlx.mlx_ptr);
	rt->mlx.mlx_ptr = NULL;
}
