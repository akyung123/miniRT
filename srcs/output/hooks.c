#include <stdlib.h>
#include "mlx.h"
#include "output.h"
#include "move.h"

int	close_hook(void *param)
{
	t_minirt	*rt;

	rt = (t_minirt *)param;
	cleanup_mlx(rt);
	free_scene(&rt->scene);
	exit(EXIT_SUCCESS);
	return (0);
}

int	key_hook(int keycode, void *param)
{
	if (keycode == KEY_ESC)
		return (close_hook(param));
	move_key_release(keycode);
	return (0);
}

int	expose_hook(void *param)
{
	t_minirt	*rt;

	rt = (t_minirt *)param;
	mlx_put_image_to_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr,
		rt->mlx.img_ptr, 0, 0);
	return (0);
}
