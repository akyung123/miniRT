/* 1단계 러너 — cases/01-pixels/ 안의 케이스 파일들
 *
 * 창을 띄우지 않습니다. 가짜 t_mlx 에 버퍼를 물려서
 * "메모리에 제대로 썼는가"만 봅니다. mlx 링크 불필요.
 */
#include <stdlib.h>
#include <string.h>
#include "miniRT.h"
#include "t.h"
#include "case.h"

void	my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color);
void	draw_circle(t_mlx *mlx, int cx, int cy, int r, int color);
void	draw_square(t_mlx *mlx, int x1, int x2, int y1, int y2, int color);
int		create_argb(int a, int r, int g, int b);

#define CANARY 0xDEADBEEF

static t_mlx	g_img;

static void	new_image(int pad)
{
	unsigned int	*p;
	int				n;

	free(g_img.addr);
	memset(&g_img, 0, sizeof g_img);
	g_img.bpp = 32;
	g_img.size_line = WIN_WIDTH * 4 + pad;
	g_img.addr = malloc((size_t)g_img.size_line * WIN_HEIGHT);
	p = (unsigned int *)g_img.addr;
	n = (g_img.size_line / 4) * WIN_HEIGHT;
	while (n-- > 0)
		p[n] = CANARY;
}

static unsigned int	px_at(int x, int y)
{
	return (*(unsigned int *)(g_img.addr + y * g_img.size_line
			+ x * (g_img.bpp / 8)));
}

static int	touched(void)
{
	int	cnt;
	int	x;
	int	y;

	cnt = 0;
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
			if (px_at(x, y) != CANARY)
				cnt++;
	}
	return (cnt);
}

static int	xy(const char *s, int *x, int *y)
{
	return (sscanf(s, "%d,%d", x, y) == 2);
}

#include "runner.h"

static const char	*g_title = "1단계 - 이미지 버퍼와 2D 도형";
static const char	*g_dir = "cases/01-pixels";

static int	handle(t_case *c)
{
	int	x;
	int	y;
	int	x2;
	int	y2;
	int	at;

	at = cs_arrow(c);
	if (at > 0)
		read_tol(c, at);
	if (cs_is(c->tok[0], "img"))
		return (new_image(atoi(c->tok[1])), 1);
	if (cs_is(c->tok[0], "put") && xy(c->tok[1], &x, &y))
		return (my_mlx_pixel_put(&g_img, x, y, (int)cs_hex(c->tok[2])), 1);
	if (cs_is(c->tok[0], "circle") && xy(c->tok[1], &x, &y))
		return (draw_circle(&g_img, x, y, atoi(c->tok[2]),
				(int)cs_hex(c->tok[3])), 1);
	if (cs_is(c->tok[0], "square") && xy(c->tok[1], &x, &x2)
		&& xy(c->tok[2], &y, &y2))
		return (draw_square(&g_img, x, x2, y, y2, (int)cs_hex(c->tok[3])), 1);
	if (cs_is(c->tok[0], "px") && at > 0 && xy(c->tok[1], &x, &y))
	{
		if (cs_is(c->tok[at], "EMPTY"))
			check_hex(px_at(x, y), "0xDEADBEEF", c->raw);
		else
			check_hex(px_at(x, y), c->tok[at], c->raw);
		return (1);
	}
	if (cs_is(c->tok[0], "count") && at > 0)
		return (check_num(touched(), c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "argb") && at > 0)
		return (check_hex((unsigned int)create_argb(atoi(c->tok[1]),
					atoi(c->tok[2]), atoi(c->tok[3]), atoi(c->tok[4])),
				c->tok[at], c->raw), 1);
	return (0);
}
