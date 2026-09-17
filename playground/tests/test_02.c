/* 2단계 러너 — cases/02-color/ */
#include "miniRT.h"
#include "t.h"
#include "case.h"

int		color_to_int(t_color c);
t_color	color_lerp(t_color a, t_color b, double t);

static t_color	vec(const char *s)
{
	t_color	c;

	cs_vec(s, &c.x, &c.y, &c.z);
	return (c);
}

#include "runner.h"

static const char	*g_title = "2단계 - 색";
static const char	*g_dir = "cases/02-color";

static int	handle(t_case *c)
{
	t_color	r;
	int		at;

	at = cs_arrow(c);
	if (at < 0)
		return (0);
	read_tol(c, at);
	if (cs_is(c->tok[0], "to_int"))
		return (check_hex((unsigned int)color_to_int(vec(c->tok[1])),
				c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "alpha"))
		return (check_num((unsigned int)color_to_int(vec(c->tok[1])) >> 24,
				c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "lerp"))
	{
		r = color_lerp(vec(c->tok[1]), vec(c->tok[2]), cs_num(c->tok[3]));
		return (check_vec(r.x, r.y, r.z, c->tok[at], c->raw), 1);
	}
	return (0);
}
