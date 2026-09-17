/* 11단계 헤드리스 프로브 — mlx 없이 진짜 파서 + 내 render_pixel
 *
 *   rt_probe [-p] [-g N] [-o out.bmp] scene.rt
 *     -p        파싱만 (누수 검사용)
 *     -g N      N 픽셀마다 한 번만 렌더 (ASAN 빌드에서 빠르게 훑기). 기본 1
 *     -o file   24bit BMP 로 저장 (-g 1 일 때만)
 *
 * stdout 한 줄:  PROBE parse=1 sp=.. pl=.. cy=.. ms=.. px=.. bad=.. hit=..
 *   bad = NaN/inf 이거나 [0,1] 밖인 픽셀 수
 *   hit = 광선이 도형에 맞은 픽셀 수 (0 이면 '하늘만' 그려진 씬)
 * 종료 코드: 0 정상, 3 파서가 거부, 4 bad 픽셀 있음
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miniRT.h"

static void	put32(unsigned char *p, unsigned int v)
{
	p[0] = v & 0xFF;
	p[1] = (v >> 8) & 0xFF;
	p[2] = (v >> 16) & 0xFF;
	p[3] = (v >> 24) & 0xFF;
}

static int	bad_color(t_color c)
{
	return (!isfinite(c.x) || !isfinite(c.y) || !isfinite(c.z)
		|| c.x < 0.0 || c.x > 1.0 || c.y < 0.0 || c.y > 1.0
		|| c.z < 0.0 || c.z > 1.0);
}

static unsigned char	to8(double v)
{
	if (!(v >= 0.0))
		return (v != v ? 255 : 0);
	if (v > 1.0)
		return (255);
	return ((unsigned char)(v * 255.0));
}

static void	count_objects(t_scene *s, int n[3])
{
	t_object	*o;

	n[0] = 0;
	n[1] = 0;
	n[2] = 0;
	o = s->objects;
	while (o)
	{
		if (o->type >= OBJ_SPHERE && o->type <= OBJ_CYLINDER)
			n[o->type]++;
		o = o->next;
	}
}

int	main(int argc, char **argv)
{
	int				parse_only = 0;
	int				step = 1;
	const char		*bmp = NULL;
	const char		*path = NULL;
	t_scene			scene;
	int				n[3];
	int				i;

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-p"))
			parse_only = 1;
		else if (!strcmp(argv[i], "-g") && i + 1 < argc)
			step = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-o") && i + 1 < argc)
			bmp = argv[++i];
		else
			path = argv[i];
	}
	if (!path || step < 1)
		return (fprintf(stderr, "usage: rt_probe [-p] [-g N] [-o out.bmp] scene.rt\n"), 2);
	if (!parse_scene(path, &scene))
	{
		printf("PROBE parse=0\n");
		return (3);
	}
	count_objects(&scene, n);
	if (parse_only)
	{
		printf("PROBE parse=1 sp=%d pl=%d cy=%d\n", n[0], n[1], n[2]);
		free_scene(&scene);
		return (0);
	}
	{
		int				row = (WIN_WIDTH * 3 + 3) & ~3;
		unsigned char	*img = NULL;
		long			px = 0, bad = 0, hit = 0;
		int				x, y;
		clock_t			t0;
		double			ms;
		t_hit			rec;

		if (bmp && step == 1)
			img = calloc((size_t)row * WIN_HEIGHT, 1);
		t0 = clock();
		for (y = 0; y < WIN_HEIGHT; y += step)
			for (x = 0; x < WIN_WIDTH; x += step)
			{
				t_color	c = render_pixel(&scene, x, y);

				px++;
				bad += bad_color(c);
				if (img)
				{
					unsigned char *p = img + (WIN_HEIGHT - 1 - y) * row + x * 3;
					p[0] = to8(c.z);
					p[1] = to8(c.y);
					p[2] = to8(c.x);
				}
			}
		ms = (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
		/* 시간 측정이 끝난 뒤 따로: 도형에 맞은 픽셀 수 */
		for (y = 0; y < WIN_HEIGHT; y += step)
			for (x = 0; x < WIN_WIDTH; x += step)
				hit += hit_scene(&scene,
						camera_ray(&scene.camera, x, y), T_MAX, &rec);
		if (img)
		{
			unsigned char	hdr[54];
			FILE			*f = fopen(bmp, "wb");

			memset(hdr, 0, sizeof hdr);
			hdr[0] = 'B';
			hdr[1] = 'M';
			put32(hdr + 2, 54 + row * WIN_HEIGHT);
			put32(hdr + 10, 54);
			put32(hdr + 14, 40);
			put32(hdr + 18, WIN_WIDTH);
			put32(hdr + 22, WIN_HEIGHT);
			hdr[26] = 1;
			hdr[28] = 24;
			put32(hdr + 34, row * WIN_HEIGHT);
			if (f)
			{
				fwrite(hdr, 1, 54, f);
				fwrite(img, 1, (size_t)row * WIN_HEIGHT, f);
				fclose(f);
			}
			free(img);
		}
		printf("PROBE parse=1 sp=%d pl=%d cy=%d ms=%.1f px=%ld bad=%ld hit=%ld\n",
			n[0], n[1], n[2], ms, px, bad, hit);
		free_scene(&scene);
		return (bad ? 4 : 0);
	}
}
