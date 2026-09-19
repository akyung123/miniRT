/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tangent_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

/* 범프가 법선을 밀 방향(접선 기저)을 만든다.
 * uv.c 의 (u, v) 를 따라 표면이 움직이는 방향이라야 무늬가 표면에
 * 붙어 보인다. 즉 t = ∂p/∂u, b = ∂p/∂v 방향이다.
 */

/* 평면: 법선에 수직이면 어느 쪽이든 된다 (uv.c 의 plane_uv 와 같은 축). */
static t_tb	plane_tb(t_vec3 n)
{
	t_tb	tb;

	tb.t = vec3_normalize(vec3_cross(stable_helper(n), n));
	tb.b = vec3_cross(n, tb.t);
	return (tb);
}

/* 구: u 가 경도라서 ∂p/∂u 는 (-z, 0, x) 방향이다. */
static t_tb	sphere_tb(t_sphere *sp, t_vec3 n, t_vec3 p)
{
	t_vec3	d;
	t_vec3	east;
	t_tb	tb;

	d = vec3_normalize(vec3_sub(p, sp->center));
	east.x = -d.z;
	east.y = 0.0;
	east.z = d.x;
	if (vec3_length(east) < 1e-8)
		return (plane_tb(n));
	tb.t = vec3_normalize(east);
	tb.b = vec3_cross(n, tb.t);
	return (tb);
}

/* 실린더 / 원뿔: u 가 축 둘레 각도라서 ∂p/∂u 는 축과 반경의 외적이다.
 * 뚜껑(법선이 축과 나란함)에서는 반경이 0이 되므로 평면 기저로 넘긴다.
 */
static t_tb	axis_tb(t_vec3 axis, t_vec3 center, t_vec3 n, t_vec3 p)
{
	t_vec3	cp;
	t_vec3	radial;
	t_tb	tb;

	cp = vec3_sub(p, center);
	radial = vec3_sub(cp, vec3_scale(axis, vec3_dot(cp, axis)));
	if (vec3_length(radial) < 1e-8)
		return (plane_tb(n));
	tb.t = vec3_normalize(vec3_cross(axis, radial));
	tb.b = vec3_cross(n, tb.t);
	return (tb);
}

t_tb	object_tb(t_object *obj, t_hit *rec)
{
	if (obj->type == OBJ_PLANE)
		return (plane_tb(rec->normal));
	if (obj->type == OBJ_SPHERE)
		return (sphere_tb(&obj->data.sphere, rec->normal, rec->point));
	if (obj->type == OBJ_CYLINDER)
		return (axis_tb(obj->data.cylinder.axis, obj->data.cylinder.center,
				rec->normal, rec->point));
	return (axis_tb(obj->data.cone.axis, obj->data.cone.center,
			rec->normal, rec->point));
}
