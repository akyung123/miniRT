/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "output.h"

void	cleanup_mlx(t_minirt *rt)
{
	if (rt->mlx.img_ptr)
		mlx_destroy_image(rt->mlx.mlx_ptr, rt->mlx.img_ptr);
	if (rt->mlx.win_ptr)
		mlx_destroy_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr);
}
