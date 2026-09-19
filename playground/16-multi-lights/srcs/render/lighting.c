/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 21:40:00 by akkim            ###   ########.fr       */
/* ************************************************************************** */

#include "render.h"

double	diffuse_factor(t_vec3 normal, t_vec3 light_dir)
{
	double	k;

	k = vec3_dot(normal, light_dir);
	if (k < 0.0)
		return (0.0);
	return (k);
}

t_color	apply_ambient(t_color obj, t_color amb, double ratio)
{
	return (vec3_scale(vec3_mul(obj, amb), ratio));
}

static t_color	light_diffuse(t_scene *scene, t_hit *rec, t_light *light)
{
	t_vec3	light_dir;
	t_color	black;
	double	k;

	black.x = 0.0;
	black.y = 0.0;
	black.z = 0.0;
	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	k = diffuse_factor(rec->normal, light_dir);
	if (k <= 0.0 || in_shadow(scene, rec->point, light->position))
		return (black);
	return (vec3_scale(vec3_mul(rec->color, light->color),
			light->brightness * k));
}

t_color	lighting(t_scene *scene, t_hit *rec)
{
	t_color	out;
	t_light	*light;

	out = apply_ambient(rec->color, scene->ambient.color,
			scene->ambient.ratio);
	light = scene->lights;
	while (light)
	{
		out = vec3_add(out, light_diffuse(scene, rec, light));
		light = light->next;
	}
	return (out);
}
