#include <stdio.h>
#include "mlx.h"
#include "output.h"

/* 00단계에서 틀렸던 네 군데를 전부 고친 판본이다.
 *   - t_mlx 를 값으로 갖는 t_minirt 를 받는다 (할당 안 된 포인터가 없다)
 *   - mlx_new_window 의 반환값을 win_ptr 에 담는다
 *   - mlx_new_image 의 첫 인자는 img_ptr 이 아니라 mlx_ptr 이다
 *   - put_image 의 마지막 두 인자는 놓을 위치이므로 0, 0 이다 (render_all)
 */

int	init_mlx(t_minirt *rt)
{
	rt->mlx.mlx_ptr = mlx_init();
	if (!rt->mlx.mlx_ptr)
		return (0);
	rt->mlx.win_ptr = mlx_new_window(rt->mlx.mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT, "miniRT - 02 color");
	if (!rt->mlx.win_ptr)
		return (0);
	rt->mlx.img_ptr = mlx_new_image(rt->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!rt->mlx.img_ptr)
		return (0);
	rt->mlx.addr = mlx_get_data_addr(rt->mlx.img_ptr, &rt->mlx.bpp,
			&rt->mlx.size_line, &rt->mlx.endian);
	if (!rt->mlx.addr)
		return (0);
	return (1);
}
