/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
이벤트 → 상태 변경 → 재렌더 (10단계)

  키 누름  ─→ held[동작] = 1 ┐
  키 뗌    ─→ held[동작] = 0 ┤
                             ▼
  매 프레임 (mlx_loop_hook) on_frame
     ├ 눌린 게 있다 → 카메라 갱신 → 저해상도 미리보기 → dirty = 1
     └ 다 뗐는데 dirty → 전체 해상도로 한 번 → dirty = 0

키 훅 안에서 바로 렌더하지 않는 이유
  렌더가 오래 걸리는 동안 들어온 키 이벤트가 큐에 쌓여서, 손을 뗀 뒤에도 한참 계속 움직인다
  훅은 held[] 에 적기만 하고 즉시 돌아가니 입력이 밀리지 않는다
*/

// 키코드 → 동작 번호. 이동/회전 키가 아니면 -1
static int	key_to_action(int keycode)
{
	static const int	keys[ACT_COUNT] = {KEY_W, KEY_S, KEY_A, KEY_D,
		KEY_E, KEY_Q, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN};
	int					act;

	act = 0;
	while (act < ACT_COUNT)
	{
		if (keys[act] == keycode)
			return (act);
		act++;
	}
	return (-1);
}

// 지금 카메라를 .rt 의 C 줄 형식으로 찍는다
// 마음에 드는 구도에서 P → 복사해서 씬 파일에 붙여넣기 (디버깅 씬 만들 때 편하다)
static void	print_camera(t_camera *cam)
{
	printf("C %.4f,%.4f,%.4f %.4f,%.4f,%.4f %g\n",
		cam->position.x, cam->position.y, cam->position.z,
		cam->orientation.x, cam->orientation.y, cam->orientation.z,
		cam->fov);
}

int	key_press(int keycode, t_app *app)
{
	int	act;

	if (keycode == KEY_ESC)
		return (close_window(app));
	if (keycode == KEY_R)
	{
		app->mini.scene.camera = app->home;
		app->dirty = 1;
	}
	else if (keycode == KEY_P)
		print_camera(&app->mini.scene.camera);
	act = key_to_action(keycode);
	if (act >= 0)
		app->held[act] = 1;
	return (0);
}

int	key_release(int keycode, t_app *app)
{
	int	act;

	act = key_to_action(keycode);
	if (act >= 0)
		app->held[act] = 0;
	return (0);
}

int	on_frame(t_app *app)
{
	t_mlx	*mlx;

	mlx = &app->mini.mlx;
	if (apply_held(&app->mini.scene.camera, app->held))
	{
		render_scaled(&app->mini, PREVIEW);
		app->dirty = 1;
	}
	else if (app->dirty)
	{
		render_scaled(&app->mini, 1);
		app->dirty = 0;
	}
	else
		return (0);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	return (0);
}

// 창 닫기 (빨간 X, ESC 공용): mlx 자원 → 파서가 malloc한 도형 리스트 순서로 정리
int	close_window(t_app *app)
{
	t_mlx	*mlx;

	mlx = &app->mini.mlx;
	mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	free_scene(&app->mini.scene);
	exit(0);
	return (0);
}

// 다른 창에 가려졌다 다시 보일 때 이미지를 다시 올린다 (다시 렌더할 필요는 없다)
int	expose(t_app *app)
{
	t_mlx	*mlx;

	mlx = &app->mini.mlx;
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	return (0);
}
