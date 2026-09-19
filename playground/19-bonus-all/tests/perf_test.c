/* 이미지 버퍼 캐싱(render_all) vs mlx_pixel_put 매 픽셀 호출(render_naive)
 * 사이의 렌더링 표시 속도를 비교하는 데모. 제출 대상 아님.
 * render_pixel은 tests/render_pixel_stub.c의 그라디언트 스텁 사용 -
 * 계산량은 두 방식이 동일하니, 시간 차이는 순수하게 "픽셀 표시 방식"
 * 차이만 반영함.
 *
 * 빌드+실행: srcs/output 에서 `make perf`
 */
#include <stdio.h>
#include <sys/time.h>
#include "mlx.h"
#include "output.h"

static double	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0);
}

static void	render_naive(t_minirt *rt)
{
	int		x;
	int		y;
	t_color	color;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			color = render_pixel(&rt->scene, x, y);
			mlx_pixel_put(rt->mlx.mlx_ptr, rt->mlx.win_ptr, x, y,
				color_to_int(color));
			x++;
		}
		y++;
	}
}

static void	print_result(double t_cached, double t_naive)
{
	printf("=== 렌더링 표시 방식 비교 (%dx%d, %d픽셀) ===\n",
		WIN_WIDTH, WIN_HEIGHT, WIN_WIDTH * WIN_HEIGHT);
	printf("이미지 버퍼 캐싱 (put_image 1회):  %8.2f ms\n", t_cached);
	printf("mlx_pixel_put 매 픽셀 호출:        %8.2f ms\n", t_naive);
	if (t_cached > 0.0)
		printf("-> %.1f배 차이\n", t_naive / t_cached);
	printf("(지금 창에 보이는 건 mlx_pixel_put 결과. ESC로 종료)\n");
}

int	main(int argc, char **argv)
{
	t_minirt	rt;
	double		t0;
	double		t_cached;
	double		t_naive;

	if (argc != 2)
		return (printf("usage: %s <scene>.rt\n", argv[0]), 1);
	if (!parse_scene(argv[1], &rt.scene))
		return (1);
	if (!init_mlx(&rt))
		return (free_scene(&rt.scene), 1);
	t0 = now_ms();
	render_all(&rt);
	t_cached = now_ms() - t0;
	t0 = now_ms();
	render_naive(&rt);
	t_naive = now_ms() - t0;
	print_result(t_cached, t_naive);
	fflush(stdout);
	mlx_hook(rt.mlx.win_ptr, 17, 0, close_hook, &rt);
	mlx_key_hook(rt.mlx.win_ptr, key_hook, &rt);
	mlx_loop(rt.mlx.mlx_ptr);
	return (0);
}
