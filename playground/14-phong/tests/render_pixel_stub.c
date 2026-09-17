/* output 모듈 자체 테스트용 임시 render_pixel 구현.
 * akkim의 실제 형태/색상 계산이 들어오면 이 파일은 삭제.
 * (srcs/ 밖에 둬서 루트 Makefile 와일드카드에 안 잡히게 함 - 진짜
 * 구현이랑 심볼 충돌 안 나도록)
 */
#include "miniRT.h"

t_color	render_pixel(t_scene *scene, int x, int y)
{
	t_color	c;

	(void)scene;
	c.x = (double)x / (double)WIN_WIDTH;
	c.y = (double)y / (double)WIN_HEIGHT;
	c.z = 0.5;
	return (c);
}
