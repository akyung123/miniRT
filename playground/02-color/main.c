#include <stdio.h>
#include "mlx.h"
#include "output.h"

int	main(int argc, char **argv)
{
	t_minirt	rt;

	(void)argc;
	(void)argv;
	rt.scene = NULL;
	if (!init_mlx(&rt))
	{
		printf("Error\nmlx init failed\n");
		return (1);
	}
	render_all(&rt);
	mlx_key_hook(rt.mlx.win_ptr, key_hook, &rt);
	mlx_hook(rt.mlx.win_ptr, 17, 0, close_hook, &rt);
	mlx_expose_hook(rt.mlx.win_ptr, expose_hook, &rt);
	mlx_loop(rt.mlx.mlx_ptr);
	return (0);
}
