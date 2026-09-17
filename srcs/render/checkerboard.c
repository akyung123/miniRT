/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 20:30:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 20:30:00 by akkim            ###   ########.fr       */
/*                                                                            */
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

static int	world_check(t_vec3 p, double scale)
{
	int	ix;
	int	iy;
	int	iz;

	ix = (int)floor(p.x / scale);
	iy = (int)floor(p.y / scale);
	iz = (int)floor(p.z / scale);
	return ((ix + iy + iz) & 1);
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
		odd = world_check(rec->point, obj->checker_scale);
	if (odd)
		return (obj->checker_color);
	return (rec->color);
}
