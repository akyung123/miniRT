/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 21:30:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 21:30:00 by akkim            ###   ########.fr       */
/* ************************************************************************** */

#include "render.h"

static int	check_parity(double u, double v, double scale)
{
	int	iu;
	int	iv;

	iu = (int)floor(u / scale);
	iv = (int)floor(v / scale);
	return ((iu + iv) & 1);
}

static int	plane_check(t_plane *pl, t_vec3 p, double scale)
{
	t_vec3	axis[2];
	t_vec3	helper;
	t_vec3	local;

	helper.x = 0.0;
	helper.y = 1.0;
	helper.z = 0.0;
	if (fabs(pl->normal.y) > 0.9)
	{
		helper.x = 1.0;
		helper.y = 0.0;
	}
	axis[0] = vec3_normalize(vec3_cross(helper, pl->normal));
	axis[1] = vec3_cross(pl->normal, axis[0]);
	local = vec3_sub(p, pl->point);
	return (check_parity(vec3_dot(local, axis[0]),
			vec3_dot(local, axis[1]), scale));
}

static int	sphere_check(t_sphere *sp, t_vec3 p, double scale)
{
	t_vec3	d;
	double	r;
	double	u;
	double	v;

	r = sp->diameter / 2.0;
	d = vec3_normalize(vec3_sub(p, sp->center));
	if (d.y < -1.0)
		d.y = -1.0;
	if (d.y > 1.0)
		d.y = 1.0;
	u = atan2(d.z, d.x) * r;
	v = acos(d.y) * r;
	return (check_parity(u, v, scale));
}

static int	cylinder_check(t_cylinder *cy, t_vec3 p, double scale)
{
	t_vec3	cp;
	t_vec3	radial;
	t_vec3	helper;
	t_vec3	around;
	double	m;

	cp = vec3_sub(p, cy->center);
	m = vec3_dot(cp, cy->axis);
	radial = vec3_sub(cp, vec3_scale(cy->axis, m));
	helper.x = 0.0;
	helper.y = 1.0;
	helper.z = 0.0;
	if (fabs(cy->axis.y) > 0.9)
	{
		helper.x = 1.0;
		helper.y = 0.0;
	}
	around = vec3_normalize(vec3_cross(helper, cy->axis));
	return (check_parity(atan2(vec3_dot(radial, vec3_cross(cy->axis, around)),
				vec3_dot(radial, around)) * (cy->diameter / 2.0), m, scale));
}

t_color	apply_checker(t_object *obj, t_hit *rec)
{
	int	odd;

	odd = 0;
	if (obj->type == OBJ_PLANE)
		odd = plane_check(&obj->data.plane, rec->point, obj->checker_scale);
	else if (obj->type == OBJ_SPHERE)
		odd = sphere_check(&obj->data.sphere, rec->point, obj->checker_scale);
	else
		odd = cylinder_check(&obj->data.cylinder, rec->point,
				obj->checker_scale);
	if (odd)
		return (obj->checker_color);
	return (rec->color);
}
