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

static void	print_help(void)
{
	printf("W/S 앞뒤  A/D 좌우  Q/E 아래위  ←/→ 좌우 회전  ↑/↓ 위아래 보기\n");
	printf("R 처음 카메라  P 지금 카메라를 C 줄로 출력  ESC 종료\n");
	printf("(한 프레임 이동량 MOVE_STEP=%g, 회전 TURN_DEG=%g°, 미리보기 %dx%d)\n",
		MOVE_STEP, TURN_DEG, PREVIEW, PREVIEW);
}

/*
10단계: 9단계 main 에 훅만 더 건다

  2  KeyPress    → key_press    (held[] = 1, ESC/R/P)
  3  KeyRelease  → key_release  (held[] = 0)
  17 DestroyNotify (빨간 X) → close_window
  loop_hook      → on_frame     (매 프레임 held[] 를 보고 이동 + 재렌더)

마스크 1L<<0, 1L<<1 은 Linux(X11)용. macOS mlx 는 무시한다
t_app 은 스택에 값으로 두고 주소만 넘긴다 (mlx_loop 는 돌아오지 않으니 끝까지 살아 있다)
*/
int	main(int argc, char **argv)
{
	t_app	app;

	if (argc != 2 || !has_rt_ext(argv[1]))
	{
		ft_error("usage: ./miniRT <scene>.rt");
		return (1);
	}
	app = (t_app){0};
	if (!parse_scene(argv[1], &app.mini.scene))
		return (1);
	app.home = app.mini.scene.camera;
	if (!init_minirt(&app.mini))
	{
		free_scene(&app.mini.scene);
		return (1);
	}
	rander(&app.mini);
	mlx_put_image_to_window(app.mini.mlx.mlx_ptr, app.mini.mlx.win_ptr, app.mini.mlx.img_ptr, 0, 0);
	mlx_hook(app.mini.mlx.win_ptr, 2, 1L << 0, key_press, &app);
	mlx_hook(app.mini.mlx.win_ptr, 3, 1L << 1, key_release, &app);
	mlx_hook(app.mini.mlx.win_ptr, 17, 0, close_window, &app);
	mlx_expose_hook(app.mini.mlx.win_ptr, expose, &app);
	mlx_loop_hook(app.mini.mlx.mlx_ptr, on_frame, &app);
	print_help();
	mlx_loop(app.mini.mlx.mlx_ptr);
	return (0);
}
