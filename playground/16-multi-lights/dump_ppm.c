#include <stdio.h>
#include <stdlib.h>
#include "miniRT.h"

int	main(int argc, char **argv)
{
	t_scene	scene;
	int		x;
	int		y;
	t_color	c;
	int		rgb[3];

	if (argc != 2)
	{
		ft_error("usage: ./dump_ppm <scene>.rt");
		return (EXIT_FAILURE);
	}
	if (!parse_scene(argv[1], &scene))
		return (EXIT_FAILURE);
	printf("P3\n%d %d\n255\n", WIN_WIDTH, WIN_HEIGHT);
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			c = vec3_clamp(render_pixel(&scene, x, y), 0.0, 1.0);
			rgb[0] = (int)(c.x * 255.0);
			rgb[1] = (int)(c.y * 255.0);
			rgb[2] = (int)(c.z * 255.0);
			printf("%d %d %d\n", rgb[0], rgb[1], rgb[2]);
			x++;
		}
		y++;
	}
	free_scene(&scene);
	return (EXIT_SUCCESS);
}
