/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 21:50:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 21:50:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

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

static void	tangent_basis(t_vec3 n, t_vec3 origin, t_vec3 p, t_frame *f)
{
	t_vec3	local;

	f->tb[0] = vec3_normalize(vec3_cross(stable_helper(n), n));
	f->tb[1] = vec3_cross(n, f->tb[0]);
	local = vec3_sub(p, origin);
	f->uv[0] = vec3_dot(local, f->tb[0]);
	f->uv[1] = vec3_dot(local, f->tb[1]);
}

static void	sphere_frame(t_sphere *sp, t_vec3 p, t_frame *f)
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
	f->uv[0] = atan2(d.z, d.x) * r;
	f->uv[1] = acos(d.y) * r;
	f->tb[0].x = -d.z;
	f->tb[0].y = 0.0;
	f->tb[0].z = d.x;
	if (vec3_length(f->tb[0]) < 1e-8)
		f->tb[0] = vec3_normalize(vec3_cross(stable_helper(n), n));
	else
		f->tb[0] = vec3_normalize(f->tb[0]);
	f->tb[1] = vec3_cross(n, f->tb[0]);
}

static void	cylinder_frame(t_cylinder *cy, t_vec3 p, t_frame *f)
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
		tangent_basis(n, cy->center, p, f);
		return ;
	}
	radial = vec3_sub(cp, vec3_scale(cy->axis, m));
	around = vec3_normalize(vec3_cross(stable_helper(cy->axis), cy->axis));
	f->uv[0] = atan2(vec3_dot(radial, vec3_cross(cy->axis, around)),
			vec3_dot(radial, around)) * (cy->diameter / 2.0);
	f->uv[1] = m;
	if (vec3_length(radial) < 1e-8)
		f->tb[0] = vec3_normalize(vec3_cross(stable_helper(n), n));
	else
		f->tb[0] = vec3_normalize(vec3_cross(cy->axis, radial));
	f->tb[1] = cy->axis;
}

void	object_frame(t_object *obj, t_vec3 p, t_frame *f)
{
	if (obj->type == OBJ_PLANE)
		tangent_basis(obj->data.plane.normal, obj->data.plane.point,
			p, f);
	else if (obj->type == OBJ_SPHERE)
		sphere_frame(&obj->data.sphere, p, f);
	else
		cylinder_frame(&obj->data.cylinder, p, f);
}
