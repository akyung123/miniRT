/* 10단계 헤드리스 검증: 키 입력(held[]) 시뮬레이션 → 카메라/미리보기/재파싱 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miniRT.h"
#include "t.h"

static int	finite3(t_vec3 v)
{
	return (isfinite(v.x) && isfinite(v.y) && isfinite(v.z));
}

static void	hold(t_camera *cam, t_action act, int frames)
{
	int	held[ACT_COUNT];

	memset(held, 0, sizeof held);
	held[act] = 1;
	while (frames-- > 0)
		apply_held(cam, held);
}

static double	ms_since(clock_t t0)
{
	return ((double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC);
}

static int	bad_color(t_color c)
{
	return (!finite3(c) || c.x < 0 || c.x > 1 || c.y < 0 || c.y > 1
		|| c.z < 0 || c.z > 1);
}

int	main(int argc, char **argv)
{
	t_scene		scene;
	t_camera	c0;
	t_camera	c;
	t_view		v0;
	t_minirt	mini;
	char		b[256];

	if (argc != 2 || !parse_scene(argv[1], &scene))
		return (fprintf(stderr, "usage: check_move scene.rt\n"), 2);
	T_BEGIN("10단계 - 카메라 이동 (헤드리스)");
	c0 = scene.camera;
	v0 = *camera_view(&c0);

	printf("\n  [ 이동 ]\n");
	c = c0; hold(&c, ACT_FORWARD, 10);
	EXPECT_NEAR(vec3_dot(vec3_sub(c.position, c0.position), v0.forward), 10 * MOVE_STEP, 1e-9, "W 10프레임 = forward 로 10*MOVE_STEP");
	c = c0; hold(&c, ACT_RIGHT, 3);
	EXPECT_NEAR(vec3_dot(vec3_sub(c.position, c0.position), v0.right), 3 * MOVE_STEP, 1e-9, "D 3프레임 = right 로 3*MOVE_STEP");
	c = c0; hold(&c, ACT_UP, 2); hold(&c, ACT_DOWN, 2);
	EXPECT_NEAR(vec3_length(vec3_sub(c.position, c0.position)), 0.0, 1e-9, "E 2 + Q 2 = 제자리");
	c = c0; hold(&c, ACT_BACK, 5);
	EXPECT_NEAR(vec3_dot(vec3_sub(c.position, c0.position), v0.forward), -5 * MOVE_STEP, 1e-9, "S 5프레임 = 뒤로");
	EXPECT_TRUE(c.orientation.x == c0.orientation.x && c.orientation.z == c0.orientation.z, "이동은 방향을 안 바꾼다");

	printf("\n  [ 회전 ]\n");
	c = c0; hold(&c, ACT_TURN_RIGHT, 5);
	EXPECT_TRUE(vec3_dot(c.orientation, v0.right) > 0.0, "→ 키 = 원래 right 쪽으로 돈다");
	c = c0; hold(&c, ACT_TURN_LEFT, 5);
	EXPECT_TRUE(vec3_dot(c.orientation, v0.right) < 0.0, "← 키 = 원래 right 반대쪽(왼쪽)으로 돈다");
	c = c0; hold(&c, ACT_LOOK_UP, 5);
	EXPECT_TRUE(vec3_dot(c.orientation, v0.up) > 0.0, "↑ 키 = 위를 본다");
	c = c0; hold(&c, ACT_TURN_LEFT, 17); hold(&c, ACT_TURN_RIGHT, 17);
	EXPECT_NEAR(vec3_length(vec3_sub(c.orientation, vec3_normalize(c0.orientation))), 0.0, 1e-9, "← 17 + → 17 = 원래 방향");
	c = c0; hold(&c, ACT_TURN_LEFT, 180);
	EXPECT_NEAR(vec3_length(vec3_sub(c.orientation, vec3_normalize(c0.orientation))), 0.0, 1e-9, "← 180프레임 x 2° = 360° 한 바퀴");
	EXPECT_NEAR(vec3_length(c.orientation), 1.0, 1e-12, "회전 후에도 단위벡터");

	printf("\n  [ pitch 제한 ±89° ]\n");
	c = c0; hold(&c, ACT_LOOK_UP, 200);
	EXPECT_NEAR(asin(c.orientation.y) * 180.0 / M_PI, PITCH_LIMIT, 1e-9, "↑ 200프레임 → 89° 에서 멈춘다");
	{
		t_view *v = camera_view(&c);
		EXPECT_TRUE(finite3(v->right) && vec3_length(v->right) > 0.99, "89° 에서 right 가 영벡터 아님");
	}
	hold(&c, ACT_TURN_LEFT, 7);
	EXPECT_NEAR(asin(c.orientation.y) * 180.0 / M_PI, PITCH_LIMIT, 1e-9, "89° 에서 좌우로 돌아도 89° 유지");
	c = c0; hold(&c, ACT_LOOK_DOWN, 200);
	EXPECT_NEAR(asin(c.orientation.y) * 180.0 / M_PI, -PITCH_LIMIT, 1e-9, "↓ 200프레임 → -89°");
	c = c0; c.orientation = (t_vec3){0, 1, 0};
	hold(&c, ACT_TURN_RIGHT, 1);
	EXPECT_TRUE(finite3(c.orientation) && c.orientation.y < 1.0, "정확히 위를 보던 씬도 첫 회전에서 NaN 없이 89° 로");

	printf("\n  [ P 출력 → 다시 파싱 ]\n");
	{
		FILE		*f;
		t_scene		s2;
		const char	*tmp = ".build/roundtrip.rt";

		c = c0; hold(&c, ACT_TURN_LEFT, 23); hold(&c, ACT_LOOK_DOWN, 11); hold(&c, ACT_FORWARD, 7);
		f = fopen(tmp, "w");
		fprintf(f, "A 0.2 255,255,255\nL 0,10,0 0.5 255,255,255\n");
		fprintf(f, "C %.4f,%.4f,%.4f %.4f,%.4f,%.4f %g\n",
			c.position.x, c.position.y, c.position.z,
			c.orientation.x, c.orientation.y, c.orientation.z, c.fov);
		fclose(f);
		EXPECT_TRUE(parse_scene(tmp, &s2), "P 로 찍은 C 줄을 파서가 받아준다");
		EXPECT_NEAR(vec3_length(vec3_sub(s2.camera.position, c.position)), 0.0, 1e-3, "  위치 복원");
		EXPECT_NEAR(vec3_dot(s2.camera.orientation, c.orientation), 1.0, 1e-6, "  방향 복원");
		free_scene(&s2);
	}

	printf("\n  [ 미리보기 render_scaled ]\n");
	mini.scene = scene;
	mini.mlx.bpp = 32;
	mini.mlx.size_line = WIN_WIDTH * 4;
	mini.mlx.addr = calloc((size_t)WIN_WIDTH * WIN_HEIGHT, 4);
	{
		int		x, y, bad = 0, n = 0;
		clock_t	t0;
		double	full_ms, prev_ms;
		unsigned int	*px = (unsigned int *)mini.mlx.addr;

		t0 = clock(); render_scaled(&mini, 1); full_ms = ms_since(t0);
		for (y = 0; y < WIN_HEIGHT; y += 13)
			for (x = 0; x < WIN_WIDTH; x += 11, n++)
				if ((int)px[y * WIN_WIDTH + x] != create_argb(render_pixel(&mini.scene, x, y)))
					bad++;
		snprintf(b, sizeof b, "block 1 = render_pixel 그대로 (%d점 비교)", n);
		EXPECT_INT(bad, 0, b);
		t0 = clock(); render_scaled(&mini, PREVIEW); prev_ms = ms_since(t0);
		bad = 0; n = 0;
		for (y = 0; y < WIN_HEIGHT; y++)
			for (x = 0; x < WIN_WIDTH; x++, n++)
			{
				int sx = (x / PREVIEW) * PREVIEW + PREVIEW / 2;
				int sy = (y / PREVIEW) * PREVIEW + PREVIEW / 2;
				if (sx >= WIN_WIDTH) sx = WIN_WIDTH - 1;
				if (sy >= WIN_HEIGHT) sy = WIN_HEIGHT - 1;
				if ((int)px[y * WIN_WIDTH + x] != create_argb(render_pixel(&mini.scene, sx, sy)))
					bad++;
			}
		snprintf(b, sizeof b, "block %d = 칸 가운데 색으로 칸 전체 (%d픽셀 전부)", PREVIEW, n);
		EXPECT_INT(bad, 0, b);
		printf("       전체 %.1f ms / 미리보기 %.1f ms (%.1f배)\n", full_ms, prev_ms, full_ms / (prev_ms > 0 ? prev_ms : 1));
		EXPECT_TRUE(prev_ms * 4 < full_ms, "미리보기가 4배 이상 빠르다");
	}

	printf("\n  [ 비행: 물체를 뚫고 지나가며 매 프레임 NaN 없음 ]\n");
	{
		int		held[ACT_COUNT];
		int		frame, x, y, bad = 0;

		memset(held, 0, sizeof held);
		mini.scene.camera = c0;
		held[ACT_FORWARD] = 1;
		held[ACT_TURN_LEFT] = 1;
		for (frame = 0; frame < 120; frame++)
		{
			if (frame == 40) { held[ACT_TURN_LEFT] = 0; held[ACT_DOWN] = 1; held[ACT_LOOK_DOWN] = 1; }
			if (frame == 80) { held[ACT_DOWN] = 0; held[ACT_LOOK_DOWN] = 0; held[ACT_LOOK_UP] = 1; }
			apply_held(&mini.scene.camera, held);
			for (y = 0; y < WIN_HEIGHT; y += 24)
				for (x = 0; x < WIN_WIDTH; x += 24)
					bad += bad_color(render_pixel(&mini.scene, x, y));
		}
		EXPECT_INT(bad, 0, "120프레임 x 825점, NaN/범위 밖 색 없음");
	}
	free(mini.mlx.addr);
	free_scene(&scene);
	return (T_END());
}
