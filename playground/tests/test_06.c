/* 6단계 러너 — cases/06-shadow/
 * sp 줄이 가로막는 도형을 등록하고, shadow 줄이 평가한 뒤 목록을 비웁니다.
 */
#include "miniRT.h"
#include "t.h"
#include "case.h"

int	shadow_test(t_vec3 p, t_vec3 light_pos, t_sphere_lite *blockers, int n);

static t_sphere_lite	g_b[16];
static int				g_n;

static t_vec3	vec(const char *s)
{
	t_vec3	v;

	cs_vec(s, &v.x, &v.y, &v.z);
	return (v);
}

#include "runner.h"

static const char	*g_title = "6단계 - 그림자";
static const char	*g_dir = "cases/06-shadow";

static int	handle(t_case *c)
{
	int	at;
	int	got;

	at = cs_arrow(c);
	if (cs_is(c->tok[0], "sp") && g_n < 16)
	{
		g_b[g_n].center = vec(c->tok[1]);
		g_b[g_n].radius = cs_num(c->tok[2]) / 2.0;
		g_n++;
		return (1);
	}
	if (cs_is(c->tok[0], "shadow") && at > 0)
	{
		got = shadow_test(vec(c->tok[1]), vec(c->tok[2]), g_b, g_n);
		g_n = 0;
		if ((cs_is(c->tok[at], "SHADOW") && got)
			|| (cs_is(c->tok[at], "LIT") && !got))
			T_OK(c->raw);
		else
			T_NG(c->raw, cs_is(c->tok[at], "SHADOW")
				? "그늘이어야 하는데 빛을 받았다"
				: "빛을 받아야 하는데 그늘이 되었다");
		return (1);
	}
	return (0);
}
