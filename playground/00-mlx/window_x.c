/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_x.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 04:04:44 by akkim             #+#    #+#             */
/*   Updated: 2026/06/23 22:57:54 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	mouse_event(int button, int x, int y, void *param)
{
	(void)param;
	printf("Mouse button %d clicked at (%d, %d)\n", button, x, y);
	return (0);
}

int	key_event(int keycode, void *param)
{
	t_mini	*vars;

	(void)vars;
	vars = (t_mini *)param;
	if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		//distroy_and_close(vars);
		exit(0);
	}
	printf("key code %d clicked.\n", keycode);
	return (0);
}

int	expose_event(void *param)
{
	(void)param;
	printf("Window needs to be redrawn (expose event triggered)\n");
	return (0);
}

int	close_window(void *param)
{
	t_mini	*vars;

	(void) vars;
	vars = (t_mini *)param;
	printf("X button clicked, exiting...\n");
	//distroy_and_close(vars);
	exit(0);
	return (0);
}
