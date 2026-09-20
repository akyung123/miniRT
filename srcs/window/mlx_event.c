/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:39:00 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/20 15:39:00 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "window.h"

void	run_mlx(t_minirt *rt)
{
	mlx_hook(rt->mlx.win_ptr, DESTROY_NOTIFY, CLOSE_MASK, close_hook, rt);
	mlx_key_hook(rt->mlx.win_ptr, key_hook, rt);
	mlx_expose_hook(rt->mlx.win_ptr, expose_hook, rt);
	mlx_loop(rt->mlx.mlx_ptr);
}
