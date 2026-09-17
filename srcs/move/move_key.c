/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "move.h"

t_move	*move_state(void)
{
	static t_move	state;

	return (&state);
}

void	move_init(t_camera *cam)
{
	t_move	*st;
	int		i;

	st = move_state();
	st->home = *cam;
	st->dirty = 0;
	i = 0;
	while (i < ACT_COUNT)
	{
		st->held[i] = 0;
		i++;
	}
}

static int	key_to_action(int keycode)
{
	int	keys[ACT_COUNT];
	int	i;

	keys[ACT_FORWARD] = KEY_W;
	keys[ACT_BACK] = KEY_S;
	keys[ACT_LEFT] = KEY_A;
	keys[ACT_RIGHT] = KEY_D;
	keys[ACT_UP] = KEY_E;
	keys[ACT_DOWN] = KEY_Q;
	keys[ACT_TURN_LEFT] = KEY_LEFT;
	keys[ACT_TURN_RIGHT] = KEY_RIGHT;
	keys[ACT_LOOK_UP] = KEY_UP;
	keys[ACT_LOOK_DOWN] = KEY_DOWN;
	i = 0;
	while (i < ACT_COUNT && keys[i] != keycode)
		i++;
	if (i == ACT_COUNT)
		return (-1);
	return (i);
}

int	move_key_press(int keycode, void *param)
{
	t_minirt	*rt;
	t_move		*st;
	int			act;

	rt = (t_minirt *)param;
	st = move_state();
	if (keycode == KEY_R)
	{
		rt->scene.camera = st->home;
		st->dirty = 1;
	}
	act = key_to_action(keycode);
	if (act >= 0)
		st->held[act] = 1;
	return (0);
}

void	move_key_release(int keycode)
{
	int	act;

	act = key_to_action(keycode);
	if (act >= 0)
		move_state()->held[act] = 0;
}
