/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_init_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "output_bonus.h"

int	init_mlx(t_minirt *rt)
{
	rt->mlx.mlx_ptr = mlx_init();
	if (!rt->mlx.mlx_ptr)
		return (ft_error("mlx_init failed"), 0);
	rt->mlx.win_ptr = mlx_new_window(rt->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT,
			"miniRT");
	if (!rt->mlx.win_ptr)
		return (ft_error("mlx_new_window failed"), 0);
	rt->mlx.img_ptr = mlx_new_image(rt->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!rt->mlx.img_ptr)
		return (ft_error("mlx_new_image failed"), 0);
	rt->mlx.addr = mlx_get_data_addr(rt->mlx.img_ptr, &rt->mlx.bpp,
			&rt->mlx.size_line, &rt->mlx.endian);
	if (!rt->mlx.addr)
		return (ft_error("mlx_get_data_addr failed"), 0);
	return (1);
}
