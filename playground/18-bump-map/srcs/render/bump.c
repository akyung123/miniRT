/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 21:50:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 21:50:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_vec3	stable_helper(t_vec3 n)
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

static void	tangent_basis(t_vec3 n, t_vec3 origin, t_vec3 p, double uv[2],
		t_vec3 tb[2])
{
	t_vec3	local;

	tb[0] = vec3_normalize(vec3_cross(stable_helper(n), n));
	tb[1] = vec3_cross(n, tb[0]);
	local = vec3_sub(p, origin);
	uv[0] = vec3_dot(local, tb[0]);
	uv[1] = vec3_dot(local, tb[1]);
}

static void	sphere_frame(t_sphere *sp, t_vec3 p, double uv[2], t_vec3 tb[2])
{
	t_vec3	d;
	t_vec3	n;
	double	r;

	r = sp->diameter / 2.0;
	n = sphere_normal(sp->center, r, p);
	d = vec3_normalize(vec3_sub(p, sp->center));
	if (d.y < -1.0)
		d.y = -1.0;
	if (d.y > 1.0)
		d.y = 1.0;
	uv[0] = atan2(d.z, d.x) * r;
	uv[1] = acos(d.y) * r;
	tb[0].x = -d.z;
	tb[0].y = 0.0;
	tb[0].z = d.x;
	if (vec3_length(tb[0]) < 1e-8)
		tb[0] = vec3_normalize(vec3_cross(stable_helper(n), n));
	else
		tb[0] = vec3_normalize(tb[0]);
	tb[1] = vec3_cross(n, tb[0]);
}

static void	cylinder_frame(t_cylinder *cy, t_vec3 p, double uv[2], t_vec3 tb[2])
{
	t_vec3	cp;
	t_vec3	radial;
	t_vec3	around;
	t_vec3	n;
	double	m;

	n = cylinder_normal(cy, p);
	cp = vec3_sub(p, cy->center);
	m = vec3_dot(cp, cy->axis);
	if (m >= cy->height / 2.0 - 1e-6 || m <= -cy->height / 2.0 + 1e-6)
	{
		tangent_basis(n, cy->center, p, uv, tb);
		return ;
	}
	radial = vec3_sub(cp, vec3_scale(cy->axis, m));
	around = vec3_normalize(vec3_cross(stable_helper(cy->axis), cy->axis));
	uv[0] = atan2(vec3_dot(radial, vec3_cross(cy->axis, around)),
			vec3_dot(radial, around)) * (cy->diameter / 2.0);
	uv[1] = m;
	if (vec3_length(radial) < 1e-8)
		tb[0] = vec3_normalize(vec3_cross(stable_helper(n), n));
	else
		tb[0] = vec3_normalize(vec3_cross(cy->axis, radial));
	tb[1] = cy->axis;
}

static void	object_frame(t_object *obj, t_vec3 p, double uv[2], t_vec3 tb[2])
{
	if (obj->type == OBJ_PLANE)
		tangent_basis(obj->data.plane.normal, obj->data.plane.point, p, uv, tb);
	else if (obj->type == OBJ_SPHERE)
		sphere_frame(&obj->data.sphere, p, uv, tb);
	else
		cylinder_frame(&obj->data.cylinder, p, uv, tb);
}

void	apply_bump(t_object *obj, t_hit *rec)
{
	double	uv[2];
	double	hu;
	double	hv;
	t_vec3	tb[2];
	t_vec3	n2;

	uv[0] = 0.0;
	uv[1] = 0.0;
	object_frame(obj, rec->point, uv, tb);
	if (vec3_dot(rec->normal, vec3_cross(tb[0], tb[1])) < 0.0)
	{
		tb[0] = vec3_negate(tb[0]);
		tb[1] = vec3_negate(tb[1]);
	}
	hu = cos(obj->bump_freq * uv[0]) * sin(obj->bump_freq * uv[1]);
	hv = sin(obj->bump_freq * uv[0]) * cos(obj->bump_freq * uv[1]);
	n2 = vec3_sub(rec->normal, vec3_scale(vec3_add(vec3_scale(tb[0], hu),
					vec3_scale(tb[1], hv)), obj->bump_strength));
	if (vec3_length(n2) < 1e-8)
		return ;
	rec->normal = vec3_normalize(n2);
}
