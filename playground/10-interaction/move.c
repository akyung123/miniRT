/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
카메라 이동 (10단계)

이동은 월드 축이 아니라 '카메라 축' 기준이어야 자연스럽다
9단계에서 만든 기저(forward / right / up)를 그대로 가져다 쓴다

  W / S  →  position ± forward * step    (보는 쪽으로 전진/후진)
  D / A  →  position ± right   * step
  E / Q  →  position ± up      * step

camera_view 는 카메라 값이 바뀌면 알아서 기저를 다시 만든다 (9단계 캐시)
→ 이동 직후에 부르는 render_pixel 도 새 기저를 쓴다
*/
void	camera_translate(t_camera *cam, t_action act, double step)
{
	t_view	*v;
	t_vec3	dir;

	v = camera_view(cam);
	if (act == ACT_FORWARD || act == ACT_BACK)
		dir = v->forward;
	else if (act == ACT_RIGHT || act == ACT_LEFT)
		dir = v->right;
	else
		dir = v->up;
	if (act == ACT_BACK || act == ACT_LEFT || act == ACT_DOWN)
		step = -step;
	cam->position = vec3_add(cam->position, vec3_scale(dir, step));
}

/*
회전 = yaw(좌우) / pitch(상하) 각도를 바꾸고 방향 벡터를 다시 만든다

방향 벡터 → 각도 (지금 보는 방향에서 출발하려고)
  yaw   = atan2(f.x, f.z)      +z 를 0° 로, +x 쪽으로 돌면 +
  pitch = asin(f.y)            수평이 0°, 위가 +

각도 → 방향 벡터
  f.x = cos(pitch) * sin(yaw)
  f.y = sin(pitch)
  f.z = cos(pitch) * cos(yaw)

          +y
           │   ● f
           │  ╱│
           │ ╱ │ sin(pitch)
           │╱  │
           ●───┴──── 수평면 위 그림자 길이 cos(pitch)
                     그 그림자를 yaw 만큼 돌린 게 (sin(yaw), cos(yaw))

yaw 가 커지면 어느 쪽으로 도나?
  d f / d yaw = (f.z, 0, -f.x)  이고  right = cross(f, (0,1,0)) = (-f.z, 0, f.x)
  → 정확히 -right 방향 → yaw + 는 '왼쪽으로 돌기'
  그래서 ← 키가 +TURN_DEG, → 키가 -TURN_DEG 다 (반대로 하면 키와 화면이 거꾸로 돈다)

pitch 는 ±PITCH_LIMIT(89°) 로 자른다
  90° 가 되면 f = (0,±1,0) → 9단계 cross 영벡터 예외를 정확히 밟는다
  (예외 처리를 해뒀어도 그 순간 world_up 이 (0,0,1)로 바뀌며 화면이 휙 돈다)
  씬이 처음부터 정확히 위를 보고 있었다면 첫 회전에서 89° 로 살짝 내려온다
*/
void	camera_turn(t_camera *cam, double yaw_deg, double pitch_deg)
{
	t_vec3	f;
	double	yaw;
	double	pitch;

	f = vec3_normalize(cam->orientation);
	if (f.y > 1.0)
		f.y = 1.0;
	if (f.y < -1.0)
		f.y = -1.0;
	yaw = atan2(f.x, f.z) + yaw_deg * M_PI / 180.0;
	pitch = asin(f.y) * 180.0 / M_PI + pitch_deg;
	if (pitch > PITCH_LIMIT)
		pitch = PITCH_LIMIT;
	if (pitch < -PITCH_LIMIT)
		pitch = -PITCH_LIMIT;
	pitch = pitch * M_PI / 180.0;
	cam->orientation = (t_vec3){cos(pitch) * sin(yaw), sin(pitch),
		cos(pitch) * cos(yaw)};
}

/*
지금 눌려 있는 동작을 한 프레임 분량씩 전부 적용

키 이벤트에서 바로 움직이지 않고 held[] 에 '눌림 상태'만 적어두는 이유
  - 키 이벤트는 누를 때/뗄 때 한 번씩만 온다 (mlx 는 자동반복을 꺼둔다)
  - 매 프레임 이 함수가 held[] 를 보고 움직이니, 누르고 있는 동안 계속 부드럽게 움직인다
  - W + D 처럼 두 키를 같이 누르면 대각선으로 간다

하나라도 움직였으면 1
*/
int	apply_held(t_camera *cam, int *held)
{
	int	moved;
	int	act;

	moved = 0;
	act = ACT_FORWARD;
	while (act <= ACT_DOWN)
	{
		if (held[act])
		{
			camera_translate(cam, act, MOVE_STEP);
			moved = 1;
		}
		act++;
	}
	if (held[ACT_TURN_LEFT] || held[ACT_TURN_RIGHT]
		|| held[ACT_LOOK_UP] || held[ACT_LOOK_DOWN])
	{
		camera_turn(cam,
			TURN_DEG * (held[ACT_TURN_LEFT] - held[ACT_TURN_RIGHT]),
			TURN_DEG * (held[ACT_LOOK_UP] - held[ACT_LOOK_DOWN]));
		moved = 1;
	}
	return (moved);
}
