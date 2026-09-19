/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

double	hit_sphere(t_ray r, t_sphere *sp, t_range range)
{
	t_vec3	oc;
	double	abc[3];
	double	radius;
	double	sqrt_d;
	double	t;

	radius = sp->diameter / 2.0;
	oc = vec3_sub(r.origin, sp->center);
	abc[0] = vec3_dot(r.direction, r.direction);
	abc[1] = 2.0 * vec3_dot(r.direction, oc);
	abc[2] = vec3_dot(oc, oc) - radius * radius;
	if (abc[1] * abc[1] - 4.0 * abc[0] * abc[2] < 0.0)
		return (-1.0);
	sqrt_d = sqrt(abc[1] * abc[1] - 4.0 * abc[0] * abc[2]);
	t = (-abc[1] - sqrt_d) / (2.0 * abc[0]);
	if (t > range.min && t < range.max)
		return (t);
	t = (-abc[1] + sqrt_d) / (2.0 * abc[0]);
	if (t > range.min && t < range.max)
		return (t);
	return (-1.0);
}
