/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "move.h"

static void	camera_translate(t_camera *cam, int act, double step)
{
	t_view	*v;
	t_vec3	dir;

	v = camera_view(cam);
	dir = v->up;
	if (act == ACT_FORWARD || act == ACT_BACK)
		dir = v->forward;
	else if (act == ACT_RIGHT || act == ACT_LEFT)
		dir = v->right;
	if (act == ACT_BACK || act == ACT_LEFT || act == ACT_DOWN)
		step = -step;
	cam->position = vec3_add(cam->position, vec3_scale(dir, step));
}

static double	clamp_pitch(double deg)
{
	if (deg > PITCH_LIMIT)
		return (PITCH_LIMIT);
	if (deg < -PITCH_LIMIT)
		return (-PITCH_LIMIT);
	return (deg);
}

static void	camera_turn(t_camera *cam, double yaw_deg, double pitch_deg)
{
	t_vec3	f;
	double	yaw;
	double	pitch;

	f = vec3_normalize(cam->orientation);
	if (f.y > 1.0)
		f.y = 1.0;
	if (f.y < -1.0)
		f.y = -1.0;
	yaw = atan2(f.x, f.z) + yaw_deg * M_PI / 180.0;
	pitch = clamp_pitch(asin(f.y) * 180.0 / M_PI + pitch_deg) * M_PI / 180.0;
	cam->orientation.x = cos(pitch) * sin(yaw);
	cam->orientation.y = sin(pitch);
	cam->orientation.z = cos(pitch) * cos(yaw);
}

int	move_apply(t_camera *cam, int *held)
{
	int	moved;
	int	act;

	moved = 0;
	act = ACT_FORWARD;
	while (act <= ACT_DOWN)
	{
		if (held[act])
		{
			camera_translate(cam, act, MOVE_STEP);
			moved = 1;
		}
		act++;
	}
	if (held[ACT_TURN_LEFT] || held[ACT_TURN_RIGHT]
		|| held[ACT_LOOK_UP] || held[ACT_LOOK_DOWN])
	{
		camera_turn(cam,
			TURN_DEG * (held[ACT_TURN_LEFT] - held[ACT_TURN_RIGHT]),
			TURN_DEG * (held[ACT_LOOK_UP] - held[ACT_LOOK_DOWN]));
		moved = 1;
	}
	return (moved);
}
