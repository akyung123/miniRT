#include <stdlib.h>
#include "mlx.h"
#include "output.h"

/* 00단계 window_x.c 를 본체 이름으로 옮긴 것.
 * keycode 를 65307 로 박아두면 macOS 에서 ESC 가 영원히 안 먹는다.
 * 두 플랫폼의 값이 다르고, 그 갈래는 output.h 의 KEY_ESC 가 이미 갖고 있다.
 */

int	close_hook(void *param)
{
	(void)param;
	exit(0);
	return (0);
}

int	key_hook(int keycode, void *param)
{
	(void)param;
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

/* 창이 가려졌다 드러나면 mlx 가 부른다. 창은 그림을 기억하지 않는다.
 */
int	expose_hook(void *param)
{
	t_minirt	*rt;

	rt = (t_minirt *)param;
	mlx_put_image_to_window(rt->mlx.mlx_ptr, rt->mlx.win_ptr,
		rt->mlx.img_ptr, 0, 0);
	return (0);
}
