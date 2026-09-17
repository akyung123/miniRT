/* 5단계 러너 — cases/05-shading/ */
#include "miniRT.h"
#include "t.h"
#include "case.h"

t_vec3	sphere_normal(t_vec3 center, double radius, t_vec3 p);
double	diffuse_factor(t_vec3 normal, t_vec3 light_dir);
t_color	apply_ambient(t_color obj, t_color amb, double ratio);

static t_vec3	vec(const char *s)
{
	t_vec3	v;

	cs_vec(s, &v.x, &v.y, &v.z);
	return (v);
}

#include "runner.h"

static const char	*g_title = "5단계 - 법선과 조명";
static const char	*g_dir = "cases/05-shading";

static int	handle(t_case *c)
{
	t_vec3	n;
	t_color	col;
	int		at;

	at = cs_arrow(c);
	if (at < 0)
		return (0);
	read_tol(c, at);
	if (cs_is(c->tok[0], "normal"))
	{
		n = sphere_normal(vec(c->tok[1]), cs_num(c->tok[2]) / 2.0,
				vec(c->tok[3]));
		return (check_vec(n.x, n.y, n.z, c->tok[at], c->raw), 1);
	}
	if (cs_is(c->tok[0], "normal_len"))
	{
		n = sphere_normal(vec(c->tok[1]), cs_num(c->tok[2]) / 2.0,
				vec(c->tok[3]));
		return (check_num(vec3_length(n), c->tok[at], c->raw), 1);
	}
	if (cs_is(c->tok[0], "diffuse"))
		return (check_num(diffuse_factor(vec(c->tok[1]), vec(c->tok[2])),
				c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "ambient"))
	{
		col = apply_ambient(vec(c->tok[1]), vec(c->tok[2]),
				cs_num(c->tok[3]));
		return (check_vec(col.x, col.y, col.z, c->tok[at], c->raw), 1);
	}
	return (0);
}
