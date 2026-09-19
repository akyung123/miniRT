/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

int	hit_scene(t_scene *scene, t_ray r, double t_max, t_hit *rec)
{
	t_object	*obj;
	int			found;

	found = 0;
	obj = scene->objects;
	while (obj)
	{
		if (hit_object(obj, r, t_max, rec))
		{
			found = 1;
			t_max = rec->t;
		}
		obj = obj->next;
	}
	if (found && vec3_dot(r.direction, rec->normal) > 0.0)
		rec->normal = vec3_negate(rec->normal);
	return (found);
}
