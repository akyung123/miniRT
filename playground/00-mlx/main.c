/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/07/25 23:58:56 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_minirt(t_mini *mini)
{
	mini->mlx_ptr = mlx_init();
	if (!mini->mlx_ptr)
		perror("MLX init failed");
	mini->win_ptr = mlx_new_window(mini->mlx_ptr, WIDTH, HEIGHT, "miniRT - akkim");
	mini->image->img_ptr = mlx_new_image(mini->mlx_ptr, WIDTH, HEIGHT);
	mini->image->addr = mlx_get_data_addr(mini->image->img_ptr, &mini->image->bpp, &mini->image->size_line, &mini->image->endian);
}

int	main(int argc, char **argv)
{
	t_mini	mini;

	(void)argv;
	(void)argc;
	init_minirt(&mini);

	draw_c2(&mini);
	mlx_put_image_to_window(mini.mlx_ptr, mini.win_ptr, mini.image->img_ptr, 0, 0);
	mlx_key_hook(mini.win_ptr, key_event, &mini);
	mlx_hook(mini.win_ptr, 17, 0, close_window, &mini);
	mlx_loop(mini.mlx_ptr);
}