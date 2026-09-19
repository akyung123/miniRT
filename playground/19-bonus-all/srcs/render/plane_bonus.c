/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

double	hit_plane(t_ray r, t_plane *pl, t_range range)
{
	double	denom;
	double	t;

	denom = vec3_dot(pl->normal, r.direction);
	if (fabs(denom) < 1e-6)
		return (-1.0);
	t = vec3_dot(pl->normal, vec3_sub(pl->point, r.origin)) / denom;
	if (t <= range.min || t >= range.max)
		return (-1.0);
	return (t);
}
