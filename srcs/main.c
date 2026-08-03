#include <stdlib.h>
#include "miniRT.h"

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
	/* TODO(렌더러): mlx 초기화 및 render loop 연결
	 * init_mlx(&minirt.mlx);
	 * render(&minirt);
	 * mlx_loop(minirt.mlx.mlx_ptr);
	 */
	free_scene(&minirt.scene);
	return (EXIT_SUCCESS);
}
