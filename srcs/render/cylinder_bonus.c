/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static t_vec3	perp(t_vec3 v, t_vec3 axis)
{
	return (vec3_sub(v, vec3_scale(axis, vec3_dot(v, axis))));
}

static int	in_height(t_ray r, t_cy *cy, double t)
{
	double	m;

	m = vec3_dot(vec3_sub(ray_at(r, t), cy->center), cy->axis);
	return (m >= -cy->half_h && m <= cy->half_h);
}

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
	if (abc[0] < 1e-12)
		return (-1.0);
	abc[1] = 2.0 * vec3_dot(d_perp, oc_perp);
	abc[2] = vec3_dot(oc_perp, oc_perp) - cy->radius * cy->radius;
	if (abc[1] * abc[1] - 4.0 * abc[0] * abc[2] < 0.0)
		return (-1.0);
	sqrt_d = sqrt(abc[1] * abc[1] - 4.0 * abc[0] * abc[2]);
	t = (-abc[1] - sqrt_d) / (2.0 * abc[0]);
	if (t > cy->range.min && t < cy->range.max && in_height(r, cy, t))
		return (t);
	t = (-abc[1] + sqrt_d) / (2.0 * abc[0]);
	if (t > cy->range.min && t < cy->range.max && in_height(r, cy, t))
		return (t);
	return (-1.0);
}

static double	hit_cap(t_ray r, t_cy *cy, double sign)
{
	t_plane	cap;
	t_vec3	from_center;
	double	t;

	cap.point = vec3_add(cy->center, vec3_scale(cy->axis, sign * cy->half_h));
	cap.normal = cy->axis;
	t = hit_plane(r, &cap, cy->range);
	if (t < 0.0)
		return (-1.0);
	from_center = vec3_sub(ray_at(r, t), cap.point);
	if (vec3_dot(from_center, from_center) > cy->radius * cy->radius)
		return (-1.0);
	return (t);
}

double	hit_cylinder(t_ray r, t_cy *cy)
{
	t_cy	c;
	double	best;
	double	t;

	c = *cy;
	best = -1.0;
	t = hit_side(r, &c);
	if (t > 0.0)
	{
		best = t;
		c.range.max = t;
	}
	t = hit_cap(r, &c, 1.0);
	if (t > 0.0)
	{
		best = t;
		c.range.max = t;
	}
	t = hit_cap(r, &c, -1.0);
	if (t > 0.0)
		best = t;
	return (best);
}
