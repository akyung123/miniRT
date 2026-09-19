/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
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

t_color	lighting(t_scene *scene, t_hit *rec)
{
	t_color	ambient;
	t_color	diffuse;
	t_vec3	light_dir;
	double	k;

	ambient = apply_ambient(rec->color, scene->ambient.color,
			scene->ambient.ratio);
	light_dir = vec3_normalize(vec3_sub(scene->light.position, rec->point));
	k = diffuse_factor(rec->normal, light_dir);
	if (k <= 0.0 || in_shadow(scene, rec->point))
		return (ambient);
	diffuse = vec3_scale(vec3_mul(rec->color, scene->light.color),
			scene->light.brightness * k);
	return (vec3_add(ambient, diffuse));
}
