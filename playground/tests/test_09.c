/* 9단계 / 최종 인수 테스트 — 실제 씬 + render_pixel
 *
 * 여기서부터는 놀이가 아니라 '과제가 통과하는가'를 봅니다.
 * 프로젝트의 진짜 파서(srcs/parsing)와 진짜 헤더(includes/miniRT.h)를
 * 그대로 쓰고, 내가 만든 render_pixel 을 붙여서 mlx 없이 렌더합니다.
 *
 * 계약 (팀 규약 - 이대로여야 제출됩니다):
 *   t_color render_pixel(t_scene *scene, int x, int y);
 *   반환값은 0.0 ~ 1.0 정규화 색. width/height 는 인자로 안 받고
 *   WIN_WIDTH / WIN_HEIGHT 매크로를 씁니다.
 */
#include <stdio.h>
#include <math.h>
#include "miniRT.h"
#include "t.h"

#define SCENES "../../scenes/"

/* 내가 구현할 함수. main 브랜치에서는 includes/output.h 에 선언되어 있지만,
 * 이 테스트는 mlx 를 안 쓰므로 여기서 직접 선언합니다. */
t_color	render_pixel(t_scene *scene, int x, int y);

static int	is_bad(double v)
{
	return (isnan(v) || isinf(v));
}

/* 씬 하나를 격자로 훑어서 색이 규약을 지키는지 본다 */
static void	render_scene(const char *rel, int expect_something)
{
	char	path[256];
	t_scene	scene;
	t_color	c;
	int		x;
	int		y;
	int		bad;
	int		distinct;
	t_color	first;

	snprintf(path, sizeof path, SCENES "%s", rel);
	if (!parse_scene(path, &scene))
	{
		T_NG(rel, "parse_scene 실패 - 파서 문제이거나 경로 문제");
		return ;
	}
	bad = 0;
	distinct = 0;
	first = render_pixel(&scene, 0, 0);
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			c = render_pixel(&scene, x, y);
			if (is_bad(c.x) || is_bad(c.y) || is_bad(c.z))
				bad++;
			else if (c.x < 0.0 || c.x > 1.0 || c.y < 0.0 || c.y > 1.0
				|| c.z < 0.0 || c.z > 1.0)
				bad++;
			if (fabs(c.x - first.x) > 1e-9 || fabs(c.y - first.y) > 1e-9
				|| fabs(c.z - first.z) > 1e-9)
				distinct++;
			x += 7;
		}
		y += 7;
	}
	if (bad == 0)
		T_OK(rel);
	else
	{
		char	b[128];

		snprintf(b, sizeof b, "%d개 픽셀이 NaN/inf 이거나 [0,1] 범위 밖", bad);
		T_NG(rel, b);
	}
	if (expect_something)
		EXPECT_TRUE(distinct > 0, "  -> 화면이 단색이 아니다 (뭔가 그려졌다)");
	free_scene(&scene);
}

static void	test_invalid_rejected(const char *rel)
{
	char	path[256];
	t_scene	scene;

	snprintf(path, sizeof path, SCENES "invalid/%s", rel);
	if (parse_scene(path, &scene))
	{
		free_scene(&scene);
		T_NG(rel, "잘못된 씬인데 파싱에 성공함");
	}
	else
		T_OK(rel);
}

int	main(void)
{
	T_BEGIN("9단계 / 최종 - 실제 씬 렌더");

	printf("\n  [ valid - 렌더되어야 함 ]\n");
	render_scene("valid/example.rt", 1);
	render_scene("valid/multi_objects.rt", 1);
	render_scene("valid/crlf_line_endings.rt", 1);
	render_scene("valid/no_trailing_newline.rt", 1);
	/* 도형 0개: 배경만 나오되 죽으면 안 됨 */
	render_scene("valid/no_objects.rt", 0);
	/* fov 0, ambient 0, brightness 0 - 0으로 나누기 지뢰 */
	render_scene("valid/boundary_values.rt", 0);

	printf("\n  [ invalid - 파서가 막아야 함 ]\n");
	test_invalid_rejected("empty_file.rt");
	test_invalid_rejected("missing_camera.rt");
	test_invalid_rejected("duplicate_ambient.rt");
	test_invalid_rejected("bad_fov.rt");
	test_invalid_rejected("bad_color_range.rt");
	test_invalid_rejected("bad_direction_vector.rt");
	test_invalid_rejected("non_integer_color.rt");
	test_invalid_rejected("wrong_field_count.rt");
	test_invalid_rejected("zero_diameter.rt");

	return (T_END());
}
