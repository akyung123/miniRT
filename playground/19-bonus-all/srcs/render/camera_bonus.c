/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static void	build_basis(t_view *v, t_vec3 orientation)
{
	t_vec3	world_up;

	v->forward = vec3_normalize(orientation);
	world_up.x = 0.0;
	world_up.y = 1.0;
	world_up.z = 0.0;
	if (fabs(vec3_dot(v->forward, world_up)) > 1.0 - 1e-6)
	{
		world_up.y = 0.0;
		world_up.z = 1.0;
	}
	v->right = vec3_normalize(vec3_cross(v->forward, world_up));
	v->up = vec3_cross(v->right, v->forward);
}

static void	build_viewport(t_view *v, double fov)
{
	double	theta;

	theta = fov * M_PI / 180.0;
	v->half_w = tan(theta / 2.0);
	v->half_h = v->half_w / ((double)WIN_WIDTH / (double)WIN_HEIGHT);
}

static int	same_camera(t_camera *a, t_camera *b)
{
	return (a->position.x == b->position.x
		&& a->position.y == b->position.y
		&& a->position.z == b->position.z
		&& a->orientation.x == b->orientation.x
		&& a->orientation.y == b->orientation.y
		&& a->orientation.z == b->orientation.z
		&& a->fov == b->fov);
}

t_view	*camera_view(t_camera *cam)
{
	static t_view	view;
	static int		ready;

	if (!ready || !same_camera(&view.cam, cam))
	{
		view.cam = *cam;
		build_basis(&view, cam->orientation);
		build_viewport(&view, cam->fov);
		ready = 1;
	}
	return (&view);
}

t_ray	camera_ray(t_camera *cam, int x, int y)
{
	t_view	*v;
	t_ray	r;
	double	u;
	double	w;

	v = camera_view(cam);
	u = (2.0 * (x + 0.5) / WIN_WIDTH - 1.0) * v->half_w;
	w = (1.0 - 2.0 * (y + 0.5) / WIN_HEIGHT) * v->half_h;
	r.origin = cam->position;
	r.direction = vec3_normalize(vec3_add(v->forward,
				vec3_add(vec3_scale(v->right, u), vec3_scale(v->up, w))));
	return (r);
}
