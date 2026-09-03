/* 2단계 검사 14개. mlx 를 링크하지 않는다 - 색 계산이 출력에서
 * 분리돼 있다는 증거이기도 하다.
 *
 *   make test && ./color_test
 */
#include <stdio.h>
#include <math.h>
#include "output.h"

static int	g_fail;

static t_color	col(double x, double y, double z)
{
	t_color	c;

	c.x = x;
	c.y = y;
	c.z = z;
	return (c);
}

static void	check_int(const char *name, int got, int want)
{
	if (got == want)
		printf("  ok   %-28s 0x%06X\n", name, got);
	else
	{
		printf("  FAIL %-28s got 0x%06X want 0x%06X\n", name, got, want);
		g_fail++;
	}
}

static void	check_col(const char *name, t_color got, t_color want)
{
	double	e;

	e = 1e-9;
	if (fabs(got.x - want.x) < e && fabs(got.y - want.y) < e
		&& fabs(got.z - want.z) < e)
		printf("  ok   %-28s (%.3f,%.3f,%.3f)\n", name, got.x, got.y, got.z);
	else
	{
		printf("  FAIL %-28s got (%.3f,%.3f,%.3f) want (%.3f,%.3f,%.3f)\n",
			name, got.x, got.y, got.z, want.x, want.y, want.z);
		g_fail++;
	}
}

static void	test_color_to_int(void)
{
	printf("color_to_int\n");
	check_int("red", color_to_int(col(1, 0, 0)), 0xFF0000);
	check_int("green", color_to_int(col(0, 1, 0)), 0x00FF00);
	check_int("blue", color_to_int(col(0, 0, 1)), 0x0000FF);
	check_int("black", color_to_int(col(0, 0, 0)), 0x000000);
	check_int("white", color_to_int(col(1, 1, 1)), 0xFFFFFF);
	check_int("mid values", color_to_int(col(0.2, 0.4, 0.6)), 0x336699);
	check_int("clamp both ends", color_to_int(col(1.5, -0.3, 0.5)), 0xFF007F);
	check_int("huge positive", color_to_int(col(99, 99, 99)), 0xFFFFFF);
	check_int("huge negative", color_to_int(col(-99, -99, -99)), 0x000000);
	check_int("top byte, white", color_to_int(col(1, 1, 1)) >> 24, 0);
	check_int("top byte, overflow", color_to_int(col(99, 99, 99)) >> 24, 0);
}

static void	test_color_lerp(void)
{
	t_color	a;
	t_color	b;

	a = col(0.0, 0.0, 0.0);
	b = col(1.0, 0.5, 0.25);
	printf("color_lerp\n");
	check_col("t = 0 gives a", color_lerp(a, b, 0.0), a);
	check_col("t = 1 gives b", color_lerp(a, b, 1.0), b);
	check_col("t = 0.5 gives middle", color_lerp(a, b, 0.5),
		col(0.5, 0.25, 0.125));
}

int	main(void)
{
	g_fail = 0;
	test_color_to_int();
	test_color_lerp();
	printf("\n");
	if (g_fail)
		printf("%d / 14 failed\n", g_fail);
	else
		printf("14 / 14 passed\n");
	return (g_fail != 0);
}
