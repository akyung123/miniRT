/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
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

/* 보너스: Phong 스펙큘러. 빛이 법선에 반사된 방향 r 과 눈 방향 v 가
 * 가까울수록 밝다. pow 의 지수가 클수록 하이라이트가 작고 날카롭다.
 */
static t_color	apply_specular(t_vec3 n, t_vec3 l, t_vec3 v, t_light *light)
{
	t_vec3	refl;
	double	k;

	refl = vec3_sub(vec3_scale(n, 2.0 * vec3_dot(n, l)), l);
	k = vec3_dot(refl, v);
	if (k < 0.0)
		k = 0.0;
	k = pow(k, SPECULAR_SHININESS);
	return (vec3_scale(light->color, light->brightness * SPECULAR_KS * k));
}

/* 광원 하나가 이 지점에 더하는 빛 (확산 + 스펙큘러). */
static t_color	light_contrib(t_scene *scene, t_hit *rec, t_light *light,
		t_vec3 view_dir)
{
	t_vec3	light_dir;
	t_color	lit;
	double	k;

	lit = vec3_scale(rec->color, 0.0);
	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	k = diffuse_factor(rec->normal, light_dir);
	if (k <= 0.0 || in_shadow(scene, rec->point, light->position))
		return (lit);
	lit = vec3_scale(vec3_mul(rec->color, light->color),
			light->brightness * k);
	return (vec3_add(lit, apply_specular(rec->normal, light_dir, view_dir,
				light)));
}

/* 보너스: 광원이 여러 개여도 각각의 기여를 그냥 더하면 된다. */
t_color	lighting(t_scene *scene, t_hit *rec, t_vec3 view_dir)
{
	t_color	out;
	t_light	*light;

	out = apply_ambient(rec->color, scene->ambient.color,
			scene->ambient.ratio);
	light = scene->lights;
	while (light)
	{
		out = vec3_add(out, light_contrib(scene, rec, light, view_dir));
		light = light->next;
	}
	return (out);
}
