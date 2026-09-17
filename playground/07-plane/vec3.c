/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/16 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// 덧셈: 성분끼리 더한다. "a만큼 간 뒤 b만큼 더 가기"
t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	out;

	out.x = a.x + b.x;
	out.y = a.y + b.y;
	out.z = a.z + b.z;
	return (out);
}

// 뺄셈: a - b 는 "b에서 a로 가는 화살표"
// 4단계에서 (광선 시작점 - 구 중심)을 구할 때 바로 이걸 쓴다
t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	out;

	out.x = a.x - b.x;
	out.y = a.y - b.y;
	out.z = a.z - b.z;
	return (out);
}

// 스칼라 곱: 방향은 그대로, 길이만 t배
// t가 음수면 방향이 반대로 뒤집힌다
t_vec3	vec3_scale(t_vec3 v, double t)
{
	t_vec3	out;

	out.x = v.x * t;
	out.y = v.y * t;
	out.z = v.z * t;
	return (out);
}

// 내적: 성분끼리 곱해서 전부 더한다 (결과는 벡터가 아니라 '숫자' 하나)
// a·b = |a||b|cos(사잇각)
//   둘 다 단위벡터면 결과가 곧 cos = 1(같은 방향) / 0(직각) / -1(반대)
double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// 외적: 두 벡터 '둘 다에 수직인' 새 벡터
// 순서를 바꾸면 부호가 뒤집힌다 (cross(a,b) = -cross(b,a))
// 평행한 두 벡터의 외적은 영벡터 -> 9단계 카메라가 정확히 위를 볼 때의 함정
t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	out;

	out.x = a.y * b.z - a.z * b.y;
	out.y = a.z * b.x - a.x * b.z;
	out.z = a.x * b.y - a.y * b.x;
	return (out);
}

// 길이: 3차원 피타고라스
// |v| = sqrt(x^2 + y^2 + z^2)
double	vec3_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

// 정규화: 방향은 그대로 두고 길이만 1로 만든다 (각 성분을 길이로 나눔)
// 길이가 0이면 0/0 = nan 이 되므로 막아둔다
t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len == 0.0)
		return (v);
	return (vec3_scale(v, 1.0 / len));
}

// 방향 뒤집기: -v
// 5단계에서 법선이 광선과 같은 쪽을 볼 때(도형 안쪽에서 맞았을 때) 쓴다
t_vec3	vec3_negate(t_vec3 v)
{
	return (vec3_scale(v, -1.0));
}

// 성분끼리 곱: (a.x*b.x, a.y*b.y, a.z*b.z)
// 내적과 달리 결과가 '벡터'. 색에 쓰면 '필터'가 된다
//   빨강(1,0,0) x 파란빛(0,0,1) = (0,0,0) → 빨간 물체는 파란빛을 하나도 반사 못 한다
// 이름/시그니처는 팀원 브랜치(feature/vec3-render-interface)와 같게 맞춤
t_vec3	vec3_mul(t_vec3 a, t_vec3 b)
{
	t_vec3	out;

	out.x = a.x * b.x;
	out.y = a.y * b.y;
	out.z = a.z * b.z;
	return (out);
}

// 각 성분을 [min, max] 안으로 자르기
// ambient + diffuse 를 더하면 1.0을 넘을 수 있어서 render_pixel 마지막에 쓴다
// (render_pixel은 0.0~1.0을 돌려주는 게 팀 규약)
t_vec3	vec3_clamp(t_vec3 v, double min, double max)
{
	t_vec3	out;

	out = v;
	if (out.x < min)
		out.x = min;
	if (out.x > max)
		out.x = max;
	if (out.y < min)
		out.y = min;
	if (out.y > max)
		out.y = max;
	if (out.z < min)
		out.z = min;
	if (out.z > max)
		out.z = max;
	return (out);
}
