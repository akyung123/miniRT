/* 8단계 러너 — cases/08-cylinder/
 * 케이스 파일은 .rt 와 같은 '지름'. 러너가 /2 해서 반지름으로 넘깁니다.
 */
#include "miniRT.h"
#include "t.h"
#include "case.h"

double	hit_cylinder(t_ray r, t_vec3 center, t_vec3 axis,
			double radius, double height, double t_min, double t_max);

static t_vec3	vec(const char *s)
{
	t_vec3	v;

	cs_vec(s, &v.x, &v.y, &v.z);
	return (v);
}

#include "runner.h"

static const char	*g_title = "8단계 - 원기둥 교차";
static const char	*g_dir = "cases/08-cylinder";

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
	t = hit_cylinder(r, vec(c->tok[3]), vec3_normalize(vec(c->tok[4])),
			cs_num(c->tok[5]) / 2.0, cs_num(c->tok[6]),
			cs_num(c->tok[7]), cs_num(c->tok[8]));
	return (check_t(t, c->tok[at], c->raw), 1);
}
