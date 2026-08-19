#include <stdlib.h>
#include "mlx.h"
#include "miniRT.h"
#include "output.h"

static int	has_rt_extension(const char *path)
{
	int	len;

	len = 0;
	while (path[len])
		len++;
	if (len < 3)
		return (0);
	return (path[len - 3] == '.' && path[len - 2] == 'r'
		&& path[len - 1] == 't');
}

static int	check_args(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_error("usage: ./miniRT <scene>.rt");
		return (0);
	}
	if (!has_rt_extension(argv[1]))
	{
		ft_error("scene file must have a .rt extension");
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (!check_args(argc, argv))
		return (EXIT_FAILURE);
	if (!parse_scene(argv[1], &minirt.scene))
		return (EXIT_FAILURE);
	if (!init_mlx(&minirt))
	{
		free_scene(&minirt.scene);
		return (EXIT_FAILURE);
	}
	render_all(&minirt);
	mlx_hook(minirt.mlx.win_ptr, 17, 0, close_hook, &minirt);
	mlx_key_hook(minirt.mlx.win_ptr, key_hook, &minirt);
	mlx_expose_hook(minirt.mlx.win_ptr, expose_hook, &minirt);
	mlx_loop(minirt.mlx.mlx_ptr);
	return (EXIT_SUCCESS);
}
