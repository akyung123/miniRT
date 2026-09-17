/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 20:12:00 by akkim            ###   ########.fr       */
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

static t_color	apply_specular(t_vec3 n, t_vec3 l, t_vec3 v, t_light *light)
{
	t_vec3	r;
	double	k;

	r = vec3_sub(vec3_scale(n, 2.0 * vec3_dot(n, l)), l);
	k = vec3_dot(r, v);
	if (k < 0.0)
		k = 0.0;
	k = pow(k, SPECULAR_SHININESS);
	return (vec3_scale(light->color, light->brightness * SPECULAR_KS * k));
}

t_color	lighting(t_scene *scene, t_hit *rec, t_vec3 view_dir)
{
	t_color	ambient;
	t_color	lit;
	t_vec3	light_dir;
	double	k;

	ambient = apply_ambient(rec->color, scene->ambient.color,
			scene->ambient.ratio);
	light_dir = vec3_normalize(vec3_sub(scene->light.position, rec->point));
	k = diffuse_factor(rec->normal, light_dir);
	if (k <= 0.0 || in_shadow(scene, rec->point))
		return (ambient);
	lit = vec3_scale(vec3_mul(rec->color, scene->light.color),
			scene->light.brightness * k);
	lit = vec3_add(lit, apply_specular(rec->normal, light_dir, view_dir,
				&scene->light));
	return (vec3_add(ambient, lit));
}
