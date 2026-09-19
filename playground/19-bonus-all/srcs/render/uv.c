/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

/* 체크무늬(ck)와 범프(bp)가 같은 표면 좌표를 써야 무늬가 어긋나지 않는다.
 * 그래서 도형별 (u, v) 계산을 여기 한 곳에 모아 둔다.
 * 단위는 전부 "길이"라서, 칸 크기나 주기를 씬 좌표계 숫자로 쓸 수 있다.
 */

/* 축과 나란하지 않은 아무 벡터. 외적이 0이 되는 걸 막는 용도. */
t_vec3	stable_helper(t_vec3 n)
{
	t_vec3	h;

	h.x = 0.0;
	h.y = 1.0;
	h.z = 0.0;
	if (fabs(n.y) > 0.9)
	{
		h.x = 1.0;
		h.y = 0.0;
	}
	return (h);
}

/* 평면: 법선에 수직인 두 축을 잡고 그 위 좌표를 그대로 쓴다. */
static t_uv	plane_uv(t_plane *pl, t_vec3 p)
{
	t_vec3	right;
	t_vec3	up;
	t_vec3	local;
	t_uv	uv;

	right = vec3_normalize(vec3_cross(stable_helper(pl->normal), pl->normal));
	up = vec3_cross(pl->normal, right);
	local = vec3_sub(p, pl->point);
	uv.u = vec3_dot(local, right);
	uv.v = vec3_dot(local, up);
	return (uv);
}

/* 구: 경도/위도 각도에 반지름을 곱해 길이 단위로 맞춘다. */
static t_uv	sphere_uv(t_sphere *sp, t_vec3 p)
{
	t_vec3	d;
	double	r;
	t_uv	uv;

	r = sp->diameter / 2.0;
	d = vec3_normalize(vec3_sub(p, sp->center));
	if (d.y < -1.0)
		d.y = -1.0;
	if (d.y > 1.0)
		d.y = 1.0;
	uv.u = atan2(d.z, d.x) * r;
	uv.v = acos(d.y) * r;
	return (uv);
}

/* 실린더와 원뿔 공통: 축 둘레 각도(u) + 축 방향 높이(v). */
static t_uv	axis_uv(t_vec3 center, t_vec3 axis, double radius, t_vec3 p)
{
	t_vec3	cp;
	t_vec3	radial;
	t_vec3	around;
	t_uv	uv;

	cp = vec3_sub(p, center);
	uv.v = vec3_dot(cp, axis);
	radial = vec3_sub(cp, vec3_scale(axis, uv.v));
	around = vec3_normalize(vec3_cross(stable_helper(axis), axis));
	uv.u = atan2(vec3_dot(radial, vec3_cross(axis, around)),
			vec3_dot(radial, around)) * radius;
	return (uv);
}

t_uv	object_uv(t_object *obj, t_vec3 p)
{
	if (obj->type == OBJ_PLANE)
		return (plane_uv(&obj->data.plane, p));
	if (obj->type == OBJ_SPHERE)
		return (sphere_uv(&obj->data.sphere, p));
	if (obj->type == OBJ_CYLINDER)
		return (axis_uv(obj->data.cylinder.center, obj->data.cylinder.axis,
				obj->data.cylinder.diameter / 2.0, p));
	return (axis_uv(obj->data.cone.center, obj->data.cone.axis,
			obj->data.cone.diameter / 2.0, p));
}
