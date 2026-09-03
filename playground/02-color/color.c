#include "output.h"

/* ===================================================================
 * 2단계의 본체. 이 파일 두 함수가 과제 전부다.
 * 지금은 껍데기라 화면이 검게 나오고 tests/color_test.c 가 다 떨어진다.
 * 채우면 창에 그라디언트가 뜨고 테스트가 초록으로 바뀐다.
 * ===================================================================
 */

/* 0~1 밖으로 나간 값을 잘라낸다.
 * 5단계에서 조명을 여러 개 더하면 1을 넘고, dot 이 음수면 0 아래로 간다.
 * 자르지 않고 * 255 하면 1.4 * 255 = 357 이 되어 빨강이 초록 채널을
 * 침범한다. 순서는 반드시 클램프 -> 곱하기 -> 캐스팅.
 */
static double	clamp(double v, double lo, double hi)
{
	(void)v;
	(void)lo;
	(void)hi;
	return (0.0);
}

/* t_color(0.0~1.0 실수) -> 0x00RRGGBB 정수.
 *
 * 만족해야 하는 것:
 *   (1,0,0)         -> 0xFF0000
 *   (0.2,0.4,0.6)   -> 0x336699
 *   (1.5,-0.3,0.5)  -> 0xFF007F    범위 밖도 안전할 것
 *   (1,1,1) >> 24   -> 0           최상위 바이트는 안 쓴다
 *
 * 256 이 아니라 255 를 곱한다. 왜인지는 playground/00-mlx/draw.c 의
 * get_color_val(clamp 0.999 * 256)과 비교해보면 나온다.
 */
int	color_to_int(t_color color)
{
	(void)color;
	(void)clamp;
	return (0);
}

/* 선형 보간. lerp(a, b, t) = (1 - t) * a + t * b
 * t=0 이면 a, t=1 이면 b, 중간이면 섞인다. 성분별로 따로 계산한다.
 * 3단계 하늘 배경과 5단계 조명 혼합이 그대로 이 함수다.
 */
t_color	color_lerp(t_color a, t_color b, double t)
{
	t_color	result;

	(void)a;
	(void)b;
	(void)t;
	result.x = 0.0;
	result.y = 0.0;
	result.z = 0.0;
	return (result);
}
