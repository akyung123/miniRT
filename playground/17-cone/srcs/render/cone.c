/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 21:45:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 21:45:00 by akkim            ###   ########.fr       */
/* ************************************************************************** */

#include "render.h"

static double	side_t(t_ray r, t_co *co, double t)
{
	double	m;

	if (t <= co->range.min || t >= co->range.max)
		return (-1.0);
	m = vec3_dot(vec3_sub(ray_at(r, t), co->apex), co->axis);
	if (m <= 1e-6 || m > co->height)
		return (-1.0);
	return (t);
}

static double	hit_side(t_ray r, t_co *co)
{
	t_vec3	oc;
	double	dv;
	double	ov;
	double	abc[3];
	double	disc;
	double	t;
	double	best;

	oc = vec3_sub(r.origin, co->apex);
	dv = vec3_dot(r.direction, co->axis);
	ov = vec3_dot(oc, co->axis);
	abc[0] = dv * dv - co->cos2 * vec3_dot(r.direction, r.direction);
	abc[1] = 2.0 * (ov * dv - co->cos2 * vec3_dot(r.direction, oc));
	abc[2] = ov * ov - co->cos2 * vec3_dot(oc, oc);
	if (fabs(abc[0]) < 1e-12)
		return (-1.0);
	disc = abc[1] * abc[1] - 4.0 * abc[0] * abc[2];
	if (disc < 0.0)
		return (-1.0);
	disc = sqrt(disc);
	best = side_t(r, co, (-abc[1] - disc) / (2.0 * abc[0]));
	t = side_t(r, co, (-abc[1] + disc) / (2.0 * abc[0]));
	if (t > 0.0 && (best < 0.0 || t < best))
		best = t;
	return (best);
}

static double	hit_base(t_ray r, t_co *co)
{
	t_plane	pl;
	t_vec3	from;
	double	t;

	pl.point = vec3_add(co->apex, vec3_scale(co->axis, co->height));
	pl.normal = co->axis;
	t = hit_plane(r, &pl, co->range);
	if (t < 0.0)
		return (-1.0);
	from = vec3_sub(ray_at(r, t), pl.point);
	if (vec3_dot(from, from) > co->radius * co->radius)
		return (-1.0);
	return (t);
}

double	hit_cone(t_ray r, t_co *co)
{
	t_co	c;
	double	best;
	double	t;

	c = *co;
	best = hit_side(r, &c);
	if (best > 0.0)
		c.range.max = best;
	t = hit_base(r, &c);
	if (t > 0.0)
		best = t;
	return (best);
}
