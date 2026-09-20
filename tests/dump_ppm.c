#include <stdio.h>
#include <stdlib.h>
#include "miniRT.h"
#include "render.h"

int	main(int argc, char **argv)
{
	t_scene	scene;
	FILE	*f;
	int		x;
	int		y;
	t_color	c;

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s scene.rt out.ppm\n", argv[0]);
		return (1);
	}
	if (!parse_scene(argv[1], &scene))
		return (1);
	f = fopen(argv[2], "wb");
	fprintf(f, "P6\n%d %d\n255\n", WIN_WIDTH, WIN_HEIGHT);
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			c = render_pixel(&scene, x, y);
			fputc((int)(c.x * 255.0), f);
			fputc((int)(c.y * 255.0), f);
			fputc((int)(c.z * 255.0), f);
			x++;
		}
		y++;
	}
	fclose(f);
	free_scene(&scene);
	return (0);
}
