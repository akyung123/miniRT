/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	init_minirt(t_minirt *mini)
{
	// t_minirt가 t_mlx를 값으로 들고 있으니 malloc이 필요 없다 (-> 대신 .)
	mini->mlx.mlx_ptr = mlx_init();
	if (!mini->mlx.mlx_ptr)
	{
		ft_error("mlx_init failed");
		return (0);
	}
	mini->mlx.win_ptr = mlx_new_window(mini->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	mini->mlx.img_ptr = mlx_new_image(mini->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!mini->mlx.win_ptr || !mini->mlx.img_ptr)
	{
		ft_error("mlx window/image failed");
		return (0);
	}
	mini->mlx.addr = mlx_get_data_addr(mini->mlx.img_ptr, &mini->mlx.bpp, &mini->mlx.size_line, &mini->mlx.endian);
	return (1);
}

// 창 닫기 (빨간 X, ESC 공용): mlx 자원 → 파서가 malloc한 도형 리스트 순서로 정리
static int	close_window(t_minirt *mini)
{
	mlx_destroy_image(mini->mlx.mlx_ptr, mini->mlx.img_ptr);
	mlx_destroy_window(mini->mlx.mlx_ptr, mini->mlx.win_ptr);
	free_scene(&mini->scene);
	exit(0);
	return (0);
}

// mlx_key_hook 은 '키를 뗄 때' 불린다 (macOS mlx 기준 keyUp)
static int	key_release(int keycode, t_minirt *mini)
{
	if (keycode == KEY_ESC)
		close_window(mini);
	return (0);
}

// 다른 창에 가려졌다 다시 보일 때 이미지를 다시 올린다 (다시 렌더할 필요는 없다)
static int	expose(t_minirt *mini)
{
	mlx_put_image_to_window(mini->mlx.mlx_ptr, mini->mlx.win_ptr, mini->mlx.img_ptr, 0, 0);
	return (0);
}

// .rt 확장자 확인: 뒤 3글자가 ".rt" 이고, 그 앞에 이름이 한 글자 이상
static int	has_rt_ext(const char *path)
{
	size_t	len;

	len = 0;
	while (path[len])
		len++;
	return (len > 3 && path[len - 3] == '.' && path[len - 2] == 'r'
		&& path[len - 1] == 't');
}

/*
9단계: demo_scene(코드로 만든 씬) → parse_scene(.rt 파일)

  ./miniRT ../../scenes/valid/example.rt

parse_scene 이 실패하면 파서가 이미 "Error\n..." 를 찍었으니 그냥 나간다
성공하면 scene->objects 는 malloc 된 연결 리스트 → 끝날 때 free_scene
*/
int	main(int argc, char **argv)
{
	t_minirt	mini;

	if (argc != 2 || !has_rt_ext(argv[1]))
	{
		ft_error("usage: ./miniRT <scene>.rt");
		return (1);
	}
	if (!parse_scene(argv[1], &mini.scene))
		return (1);
	if (!init_minirt(&mini))
	{
		free_scene(&mini.scene);
		return (1);
	}
	rander(&mini);
	mlx_put_image_to_window(mini.mlx.mlx_ptr, mini.mlx.win_ptr, mini.mlx.img_ptr, 0, 0);
	// 17 = DestroyNotify (창의 빨간 X)
	mlx_hook(mini.mlx.win_ptr, 17, 0, close_window, &mini);
	mlx_key_hook(mini.mlx.win_ptr, key_release, &mini);
	mlx_expose_hook(mini.mlx.win_ptr, expose, &mini);
	mlx_loop(mini.mlx.mlx_ptr);
	return (0);
}
