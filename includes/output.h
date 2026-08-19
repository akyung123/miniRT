#ifndef OUTPUT_H
# define OUTPUT_H

# include "miniRT.h"

# ifdef __APPLE__
#  define KEY_ESC 53
# else
#  define KEY_ESC 65307
# endif

/* render_pixel은 원래 miniRT.h에 선언될 인터페이스 함수 (PR #1 대기 중,
 * https://github.com/akyung123/miniRT/pull/1). 머지 전까지 여기서도
 * 선언해서 output 모듈이 독립적으로 컴파일되게 함 - 동일 시그니처
 * 중복 선언은 C에서 문제 없음. 머지되면 이 줄은 지워도 되고 남겨둬도 무방.
 * width/height는 인자로 안 받고 miniRT.h의 WIN_WIDTH/WIN_HEIGHT 매크로
 * 사용(norm 함수당 인자 4개 제한 때문에 5개였던 걸 3개로 줄임).
 */
t_color	render_pixel(t_scene *scene, int x, int y);

int		init_mlx(t_minirt *rt);
void	render_all(t_minirt *rt);
int		color_to_int(t_color color);
int		close_hook(void *param);
int		key_hook(int keycode, void *param);
int		expose_hook(void *param);
void	cleanup_mlx(t_minirt *rt);

#endif
