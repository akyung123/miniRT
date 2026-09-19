/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "mlx.h"
#include "output_bonus.h"

/* macOS mlx 에는 mlx_destroy_display 가 없다. 리눅스에서는 이걸
 * 안 부르면 X 연결이 그대로 남아 valgrind 가 누수로 잡는다.
 */
#ifndef __APPLE__

static void	destroy_display(void *mlx_ptr)
{
	mlx_destroy_display(mlx_ptr);
	free(mlx_ptr);
}

#else

static void	destroy_display(void *mlx_ptr)
{
	(void)mlx_ptr;
}

#endif

void	cleanup_mlx(t_minirt *rt)
{
	if (!rt->mlx.mlx_ptr)
		return ;
	if (rt->mlx.img_ptr)
		mlx_destroy_image(rt->mlx.mlx_ptr, rt->mlx.img_ptr);
	if (rt->mlx.win_ptr)
		mlx_destroy_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr);
	rt->mlx.img_ptr = NULL;
	rt->mlx.win_ptr = NULL;
	destroy_display(rt->mlx.mlx_ptr);
	rt->mlx.mlx_ptr = NULL;
}
