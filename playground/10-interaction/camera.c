/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
임의 위치 / 임의 방향 / 임의 fov 카메라 (9단계)

3단계 카메라는 '눈은 원점, -z를 본다, 오른쪽은 +x, 위는 +y' 로 고정이었다
.rt 의 C 줄은 위치와 보는 방향을 마음대로 준다
  → 화면의 오른쪽/위쪽이 월드 좌표로 어느 방향인지를 직접 만들어야 한다 = 직교기저

  forward = 카메라가 보는 방향 (파서가 이미 정규화)
  right   = normalize(cross(forward, world_up))
  up      = cross(right, forward)     ← right ⟂ forward, 둘 다 단위라 정규화 불필요

              up
              ↑
              │   ↗ forward (화면 안쪽으로)
              │ ╱
              ●────→ right
            카메라

순서 확인 (3단계 카메라와 같아야 한다)
  forward = (0,0,-1), world_up = (0,1,0)
  cross(forward, world_up) = (1,0,0)  = +x 오른쪽  ✓
  cross(right, forward)    = (0,1,0)  = +y 위      ✓
  right 의 cross 순서만 뒤집으면 right = -x, 그걸로 만든 up 도 -y → 그림이 180° 돌아간다
  두 줄 다 뒤집으면 up 은 원래대로(-(-y)) → 거울에 비친 것처럼 좌우만 반전
  (11단계에서 cam_front.rt 로 렌더해 보니 각각 원본과 픽셀 단위로 정확히 일치)
*/

// 기저 만들기. forward 가 world_up 과 평행하면 cross 가 영벡터가 되니 다른 축으로 바꾼다
static void	build_basis(t_view *v, t_vec3 orientation)
{
	t_vec3	world_up;

	v->forward = vec3_normalize(orientation);
	world_up = (t_vec3){0.0, 1.0, 0.0};
	// 정확히 위(0,1,0)나 아래(0,-1,0)를 보는 카메라
	// cross((0,1,0), (0,1,0)) = (0,0,0) → normalize 해도 0 → 모든 광선이 forward 하나로 뭉친다
	if (fabs(vec3_dot(v->forward, world_up)) > 1.0 - 1e-6)
		world_up = (t_vec3){0.0, 0.0, 1.0};
	v->right = vec3_normalize(vec3_cross(v->forward, world_up));
	v->up = vec3_cross(v->right, v->forward);
}

/*
fov → 뷰포트 반폭

카메라에서 거리 1 앞에 화면이 있다고 두면

        화면 (거리 1)
    ●─────────┬─────────●
     ╲        │        ╱
      ╲       │1      ╱        tan(θ/2) = 반폭 / 1
       ╲  θ/2 │ θ/2  ╱
        ╲     │     ╱
         ╲    │    ╱
              👁

.rt 의 fov 는 '도' 단위 '수평' 화각 → 라디안으로 바꿔서 절반의 tan
세로는 화면 비율만큼 줄인다 (800x600 이면 3/4)

fov 0 이면 tan(0) = 0 → 반폭 0 → 모든 광선이 forward 방향 (나누기가 없으니 NaN 없음)
*/
static void	build_viewport(t_view *v, double fov)
{
	double	theta;

	theta = fov * M_PI / 180.0;
	v->half_w = tan(theta / 2.0);
	v->half_h = v->half_w / ((double)WIN_WIDTH / (double)WIN_HEIGHT);
}

// 카메라 값 7개가 전부 같은가 (위치 3 + 방향 3 + fov)
static int	same_camera(t_camera *a, t_camera *b)
{
	return (a->position.x == b->position.x && a->position.y == b->position.y
		&& a->position.z == b->position.z
		&& a->orientation.x == b->orientation.x
		&& a->orientation.y == b->orientation.y
		&& a->orientation.z == b->orientation.z
		&& a->fov == b->fov);
}

/*
기저 캐시

기저는 카메라가 그대로면 48만 픽셀 전부 똑같다 → 한 번만 계산해 static 에 둔다

캐시가 '맞는지'를 무엇으로 판단하나
  ✗ scene 포인터 : 테스트(test_09.c)는 씬마다 지역변수 t_scene 을 새로 파싱하는데
                   스택 주소가 매번 같아서, 포인터로 비교하면 두 번째 씬부터 첫 씬의 카메라로 그린다
  ✓ 카메라 값 자체 : 값이 하나라도 바뀌면 다시 만든다 (10단계에서 카메라를 움직여도 자동으로 갱신)
*/
t_view	*camera_view(t_camera *cam)
{
	static t_view	view;
	static int		ready;

	if (!ready || !same_camera(&view.cam, cam))
	{
		view.cam = *cam;
		build_basis(&view, cam->orientation);
		build_viewport(&view, cam->fov);
		ready = 1;
	}
	return (&view);
}

/*
픽셀 (x, y) 를 지나는 광선

3단계와 같다. 축만 월드 축 (x, y, -z) 에서 카메라 축 (right, up, forward) 로 바뀌었다

  u = (2 * (x + 0.5) / W - 1) * half_w     -half_w ~ +half_w, 왼쪽 → 오른쪽
  v = (1 - 2 * (y + 0.5) / H) * half_h     +half_h ~ -half_h, 위 → 아래 (y 뒤집기)

  +0.5 : 픽셀의 '왼쪽 위 모서리'가 아니라 '가운데'를 지나게 한다
         그래야 가운데 열 (W-1)/2 기준으로 좌우가 정확히 대칭

  dir = normalize(forward + u * right + v * up)
*/
t_ray	camera_ray(t_camera *cam, int x, int y)
{
	t_view	*v;
	t_ray	r;
	double	u;
	double	w;

	v = camera_view(cam);
	u = (2.0 * (x + 0.5) / WIN_WIDTH - 1.0) * v->half_w;
	w = (1.0 - 2.0 * (y + 0.5) / WIN_HEIGHT) * v->half_h;
	r.origin = cam->position;
	r.direction = vec3_normalize(vec3_add(v->forward,
				vec3_add(vec3_scale(v->right, u), vec3_scale(v->up, w))));
	return (r);
}
