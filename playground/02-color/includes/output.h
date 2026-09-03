#ifndef OUTPUT_H
# define OUTPUT_H

# include "miniRT.h"

/* 본체 includes/output.h 를 그대로 옮긴 것.
 * cleanup_mlx 만 2단계에서 아직 안 쓰므로 뺐다.
 */

# ifdef __APPLE__
#  define KEY_ESC 53
# else
#  define KEY_ESC 65307
# endif

/* 렌더러가 바깥에 내놓는 유일한 함수.
 * 반환은 항상 0.0 ~ 1.0 정규화 색이고, 0~255 로 바꾸는 건 color_to_int 몫.
 */
t_color	render_pixel(t_scene *scene, int x, int y);

int		init_mlx(t_minirt *rt);
void	render_all(t_minirt *rt);
int		color_to_int(t_color color);
t_color	color_lerp(t_color a, t_color b, double t);
int		close_hook(void *param);
int		key_hook(int keycode, void *param);
int		expose_hook(void *param);

#endif
