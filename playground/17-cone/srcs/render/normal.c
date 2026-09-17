/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

t_vec3	sphere_normal(t_vec3 center, double radius, t_vec3 p)
{
	return (vec3_scale(vec3_sub(p, center), 1.0 / radius));
}

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
	return (vec3_normalize(vec3_sub(cp, vec3_scale(cy->axis, m))));
}

t_vec3	cone_normal(t_cone *cn, t_vec3 p)
{
	t_vec3	apex;
	t_vec3	axis;
	t_vec3	x;
	double	m;
	double	h;
	double	rad;
	double	cos2;

	h = cn->height;
	rad = cn->diameter / 2.0;
	axis = vec3_negate(cn->axis);
	apex = vec3_add(cn->center, vec3_scale(cn->axis, h / 2.0));
	x = vec3_sub(p, apex);
	m = vec3_dot(x, axis);
	if (m >= h - 1e-6)
		return (axis);
	cos2 = (h * h) / (h * h + rad * rad);
	return (vec3_normalize(vec3_sub(vec3_scale(axis, m),
				vec3_scale(x, cos2))));
}
