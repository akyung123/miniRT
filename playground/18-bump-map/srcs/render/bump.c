/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 21:50:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 21:50:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	apply_bump(t_object *obj, t_hit *rec)
{
	t_frame	f;
	double	hu;
	double	hv;
	t_vec3	n2;

	f.uv[0] = 0.0;
	f.uv[1] = 0.0;
	object_frame(obj, rec->point, &f);
	if (vec3_dot(rec->normal, vec3_cross(f.tb[0], f.tb[1])) < 0.0)
	{
		f.tb[0] = vec3_negate(f.tb[0]);
		f.tb[1] = vec3_negate(f.tb[1]);
	}
	hu = cos(obj->bump_freq * f.uv[0]) * sin(obj->bump_freq * f.uv[1]);
	hv = sin(obj->bump_freq * f.uv[0]) * cos(obj->bump_freq * f.uv[1]);
	n2 = vec3_sub(rec->normal, vec3_scale(vec3_add(vec3_scale(f.tb[0], hu),
					vec3_scale(f.tb[1], hv)), obj->bump_strength));
	if (vec3_length(n2) < 1e-8)
		return ;
	rec->normal = vec3_normalize(n2);
}
