/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_ray	shadow_ray(t_vec3 p, t_vec3 light_pos, double *dist)
{
	t_ray	r;
	t_vec3	to_light;

	to_light = vec3_sub(light_pos, p);
	*dist = vec3_length(to_light);
	r.origin = p;
	r.direction = vec3_normalize(to_light);
	return (r);
}

int	in_shadow(t_scene *scene, t_vec3 p, t_vec3 light_pos)
{
	t_ray		r;
	t_object	*obj;
	t_hit		tmp;
	double		dist;

	r = shadow_ray(p, light_pos, &dist);
	obj = scene->objects;
	while (obj)
	{
		if (hit_object(obj, r, dist - T_MIN, &tmp))
			return (1);
		obj = obj->next;
	}
	return (0);
}
