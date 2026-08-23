/* 4단계 러너 — cases/04-sphere/
 * 케이스 파일은 .rt 와 같은 '지름'으로 적혀 있고,
 * 러너가 /2 해서 반지름으로 넘깁니다.
 */
#include "miniRT.h"
#include "t.h"
#include "case.h"

double	hit_sphere(t_ray r, t_vec3 center, double radius,
			double t_min, double t_max);

static t_vec3	vec(const char *s)
{
	t_vec3	v;

	cs_vec(s, &v.x, &v.y, &v.z);
	return (v);
}

#include "runner.h"

static const char	*g_title = "4단계 - 구 교차";
static const char	*g_dir = "cases/04-sphere";

static int	handle(t_case *c)
{
	t_ray	r;
	double	t;
	int		at;

	at = cs_arrow(c);
	if (at < 0 || !cs_is(c->tok[0], "hit"))
		return (0);
	read_tol(c, at);
	r.origin = vec(c->tok[1]);
	r.direction = vec3_normalize(vec(c->tok[2]));
	t = hit_sphere(r, vec(c->tok[3]), cs_num(c->tok[4]) / 2.0,
			cs_num(c->tok[5]), cs_num(c->tok[6]));
	return (check_t(t, c->tok[at], c->raw), 1);
}
