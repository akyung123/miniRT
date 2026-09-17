/* 7단계 러너 — cases/07-plane/ */
#include "miniRT.h"
#include "t.h"
#include "case.h"

double	hit_plane(t_ray r, t_vec3 point, t_vec3 normal,
			double t_min, double t_max);

static t_vec3	vec(const char *s)
{
	t_vec3	v;

	cs_vec(s, &v.x, &v.y, &v.z);
	return (v);
}

#include "runner.h"

static const char	*g_title = "7단계 - 평면 교차";
static const char	*g_dir = "cases/07-plane";

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
	t = hit_plane(r, vec(c->tok[3]), vec3_normalize(vec(c->tok[4])),
			cs_num(c->tok[5]), cs_num(c->tok[6]));
	return (check_t(t, c->tok[at], c->raw), 1);
}
