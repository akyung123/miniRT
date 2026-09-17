/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVE_H
# define MOVE_H

# include "render.h"

# ifdef __APPLE__
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_Q 12
#  define KEY_E 14
#  define KEY_R 15
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_DOWN 125
#  define KEY_UP 126
# else
#  define KEY_W 119
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_Q 113
#  define KEY_E 101
#  define KEY_R 114
#  define KEY_LEFT 65361
#  define KEY_UP 65362
#  define KEY_RIGHT 65363
#  define KEY_DOWN 65364
# endif

# define MOVE_STEP 1.0
# define TURN_DEG 2.0
# define PREVIEW 4
# define PITCH_LIMIT 89.0

typedef enum e_action
{
	ACT_FORWARD,
	ACT_BACK,
	ACT_LEFT,
	ACT_RIGHT,
	ACT_UP,
	ACT_DOWN,
	ACT_TURN_LEFT,
	ACT_TURN_RIGHT,
	ACT_LOOK_UP,
	ACT_LOOK_DOWN,
	ACT_COUNT
}	t_action;

typedef struct s_move
{
	t_camera	home;
	int			held[ACT_COUNT];
	int			dirty;
}	t_move;

t_move	*move_state(void);
void	move_init(t_camera *cam);
int		move_key_press(int keycode, void *param);
void	move_key_release(int keycode);
int		move_apply(t_camera *cam, int *held);
int		move_loop_hook(void *param);

#endif
