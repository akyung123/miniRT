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

void	mini_destroy(t_mlx	*mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	// free(mlx->mlx_ptr);
}

int	init_minirt(t_minirt *mini)
{
	mini->mlx = malloc(sizeof(t_mlx));
	// display와 연결하기
	// 그래픽 시스템에 연결하고 불투명 포인터를 돌려줌
	mini->mlx->mlx_ptr = mlx_init();
	if (!mini->mlx->mlx_ptr)
	{
		perror("MLX init failed");
		return (0);
	}
	// 메모리 안에 새로운 윈도우 생성
	mini->mlx->win_ptr = mlx_new_window(mini->mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	// 메모리 안에 새로운 이미지 생성
	mini->mlx->img_ptr = mlx_new_image(mini->mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	mini->mlx->addr = mlx_get_data_addr(mini->mlx->img_ptr, &mini->mlx->bpp, &mini->mlx->size_line, &mini->mlx->endian);
	return (1);
}

// test용 함수
static void	print_vec(const char *name, t_vec3 v)
{
	printf("%-10s (%.4f, %.4f, %.4f)\n", name, v.x, v.y, v.z);
}

//
static void	check_camera(void)
{
	print_vec("(0,0)", camera_ray(0, 0).direction);
	print_vec("(400,300)", camera_ray(400, 300).direction);
	print_vec("(799,599)", camera_ray(799, 599).direction);
	printf("----\n");
}

int main(int argc, char **argv)
{
	t_minirt mini;

	(void)argv;
	(void)argc;

	// minirt setting
	check_camera();
	if (!init_minirt(&mini))
		return (0);
	// 이미지 만들기
	rander(&mini); 
	// image에 window 위에 넣기
	mlx_put_image_to_window(mini.mlx->mlx_ptr, mini.mlx->win_ptr, mini.mlx->img_ptr, 0, 0);

	mlx_loop(mini.mlx->mlx_ptr);
	// mlx destroy
	mini_destroy(mini.mlx);
}