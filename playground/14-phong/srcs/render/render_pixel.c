/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_color	sky(t_ray r)
{
	t_color	c;
	double	t;

	t = 0.5 * (r.direction.y + 1.0);
	c.x = 1.0 * (1.0 - t) + 0.5 * t;
	c.y = 1.0 * (1.0 - t) + 0.7 * t;
	c.z = 1.0 * (1.0 - t) + 1.0 * t;
	return (c);
}

t_color	render_pixel(t_scene *scene, int x, int y)
{
	t_ray	r;
	t_hit	rec;
	t_vec3	view_dir;

	r = camera_ray(&scene->camera, x, y);
	if (!hit_scene(scene, r, T_MAX, &rec))
		return (sky(r));
	view_dir = vec3_normalize(vec3_sub(scene->camera.position, rec.point));
	return (vec3_clamp(lighting(scene, &rec, view_dir), 0.0, 1.0));
}
