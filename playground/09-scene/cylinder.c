/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
원기둥 = (a) 측면  +  (b) 높이로 자르기  +  (c) 뚜껑 2개

  C = 중심 (밑면이 아니라 '가운데'), a = 축 (단위벡터), r = 반지름, h = 높이

              ┌───●───┐  ← 윗뚜껑 중심 C + a·(h/2), 법선 +a
              │   ↑a  │
              │   ●C  │  ← 축 방향 높이 m 이 -h/2 ~ +h/2 인 곳만 원기둥
              │       │
              └───●───┘  ← 아랫뚜껑 중심 C - a·(h/2), 법선 -a
                  ←r→

핵심 한 줄: 축 성분 걷어내기

  v_perp = v - (v·a)·a

  (v·a)·a 는 v를 축 위로 투영한 성분. 그걸 빼면 '축에 수직인 성분'만 남는다
  원기둥 측면 = '축으로부터 수직 거리가 r인 점' 이니까
  수직 성분만 보면 3D 원기둥이 2D 원이 되고, 4단계 구 공식이 그대로 쓰인다

         축 a
          ↑
          │   v
          │  ↗
          │ ╱
          │╱
          ●────→ v_perp   (축 성분을 빼고 남은 것)
*/

// v에서 축 방향 성분을 빼서 축에 수직인 성분만 남긴다 (a는 단위벡터여야 한다)
static t_vec3	perp(t_vec3 v, t_vec3 axis)
{
	return (vec3_sub(v, vec3_scale(axis, vec3_dot(v, axis))));
}

// (b) 광선 위 t 지점이 원기둥 높이 안인가
// m = (P - C)·a : P를 축 위에 투영한 '중심으로부터의 높이'
static int	in_height(t_ray r, t_cy *cy, double t)
{
	double	m;

	m = vec3_dot(vec3_sub(ray_at(r, t), cy->center), cy->axis);
	return (m >= -cy->half_h && m <= cy->half_h);
}

/*
(a) + (b) 측면

광선 P(t) = A + t·d 와 oc = A - C 에서 축 성분을 걷어내면
  d_perp  = d  - (d·a)a
  oc_perp = oc - (oc·a)a
축으로부터의 수직 거리가 r 이라는 조건
  |oc_perp + t·d_perp|² = r²
구와 똑같이 전개하면
  a_ = d_perp·d_perp
  b_ = 2(d_perp·oc_perp)
  c_ = oc_perp·oc_perp - r²

두 근 모두 '범위 안 + 높이 안'인지 봐야 한다
  t1이 높이 밖(뚫린 윗부분을 지나감)이어도 t2는 안쪽 벽일 수 있다
*/
static double	hit_side(t_ray r, t_cy *cy)
{
	t_vec3	d_perp;
	t_vec3	oc_perp;
	double	abc[3];
	double	sqrt_d;
	double	t;

	d_perp = perp(r.direction, cy->axis);
	oc_perp = perp(vec3_sub(r.origin, cy->center), cy->axis);
	abc[0] = vec3_dot(d_perp, d_perp);
	// a_ ≈ 0 : 광선이 축과 나란하다 → 측면과는 절대 안 만남 (뚜껑만 가능)
	// 여기서 안 막으면 아래에서 0으로 나눈다
	if (abc[0] < 1e-12)
		return (-1.0);
	abc[1] = 2.0 * vec3_dot(d_perp, oc_perp);
	abc[2] = vec3_dot(oc_perp, oc_perp) - cy->radius * cy->radius;
	if (abc[1] * abc[1] - 4.0 * abc[0] * abc[2] < 0.0)
		return (-1.0);
	sqrt_d = sqrt(abc[1] * abc[1] - 4.0 * abc[0] * abc[2]);
	// t1: 들어가는 쪽
	t = (-abc[1] - sqrt_d) / (2.0 * abc[0]);
	if (t > cy->t_min && t < cy->t_max && in_height(r, cy, t))
		return (t);
	// t2: 나오는 쪽 (원기둥 안에서 출발했거나, t1이 높이 밖일 때)
	t = (-abc[1] + sqrt_d) / (2.0 * abc[0]);
	if (t > cy->t_min && t < cy->t_max && in_height(r, cy, t))
		return (t);
	return (-1.0);
}

/*
(c) 뚜껑 하나 = 원판 = 평면 교차(7단계) + 반지름 검사

sign = +1 이면 윗뚜껑, -1 이면 아랫뚜껑
1) 뚜껑 중심 = C + a·(sign·h/2), 뚜껑 평면의 법선 = a (평면은 법선 부호와 무관)
2) hit_plane으로 t
3) 교차점이 뚜껑 중심에서 r 이내일 때만 인정 (제곱끼리 비교해서 sqrt 생략)
*/
static double	hit_cap(t_ray r, t_cy *cy, double sign)
{
	t_vec3	cap_center;
	t_vec3	from_center;
	double	t;

	cap_center = vec3_add(cy->center,
			vec3_scale(cy->axis, sign * cy->half_h));
	t = hit_plane(r, cap_center, cy->axis, cy->t_min, cy->t_max);
	if (t < 0.0)
		return (-1.0);
	from_center = vec3_sub(ray_at(r, t), cap_center);
	if (vec3_dot(from_center, from_center) > cy->radius * cy->radius)
		return (-1.0);
	return (t);
}

/*
원기둥 교차 (test_08.c 계약): 측면, 윗뚜껑, 아랫뚜껑 중 가장 가까운 t

4단계 render_pixel의 closest 기법을 원기둥 안에서 한 번 더 쓴다
  하나 맞을 때마다 t_max를 그 t로 줄이면, 다음 부품은 더 가까울 때만 맞는다
  → 마지막에 남은 best가 셋 중 가장 가까운 것

center는 '중심', axis는 단위벡터(러너와 파서가 정규화해서 준다)
맞으면 t (양수), 안 맞으면 -1.0
*/
double	hit_cylinder(t_ray r, t_vec3 center, t_vec3 axis,
			double radius, double height, double t_min, double t_max)
{
	t_cy	cy;
	double	best;
	double	t;

	cy = (t_cy){center, axis, radius, height / 2.0, t_min, t_max};
	best = -1.0;
	t = hit_side(r, &cy);
	if (t > 0.0)
	{
		best = t;
		cy.t_max = t;
	}
	t = hit_cap(r, &cy, 1.0);
	if (t > 0.0)
	{
		best = t;
		cy.t_max = t;
	}
	t = hit_cap(r, &cy, -1.0);
	if (t > 0.0)
		best = t;
	return (best);
}

/*
원기둥 표면점 P의 법선

m = (P - C)·a  (P의 축 방향 높이)
  m ≈ +h/2 → 윗뚜껑 위의 점 → 법선 +a
  m ≈ -h/2 → 아랫뚜껑 위의 점 → 법선 -a
  그 외    → 측면 → 축에서 P로 뻗는 수직 방향 = perp(P - C)를 정규화

뚜껑 위의 점은 m이 정확히 ±h/2 여야 하지만 부동소수점 오차가 있으니 1e-6 여유를 둔다
*/
t_vec3	cylinder_normal(t_cylinder *cy, t_vec3 p)
{
	t_vec3	cp;
	double	m;
	double	half_h;

	cp = vec3_sub(p, cy->center);
	m = vec3_dot(cp, cy->axis);
	half_h = cy->height / 2.0;
	if (m >= half_h - 1e-6)
		return (cy->axis);
	if (m <= -half_h + 1e-6)
		return (vec3_negate(cy->axis));
	return (vec3_normalize(perp(cp, cy->axis)));
}
