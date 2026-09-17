/* 3단계 러너 — cases/03-ray/ */
#include <math.h>
#include "miniRT.h"
#include "t.h"
#include "case.h"

static t_vec3	vec(const char *s)
{
	t_vec3	v;

	cs_vec(s, &v.x, &v.y, &v.z);
	return (v);
}

#include "runner.h"

static const char	*g_title = "3단계 - 벡터와 광선";
static const char	*g_dir = "cases/03-ray";

static void	sign_check(double v, const char *want, const char *what)
{
	if ((cs_is(want, "POS") && v > 0) || (cs_is(want, "NEG") && v < 0))
		T_OK(what);
	else
		T_NG(what, cs_is(want, "POS") ? "양수여야 함" : "음수여야 함");
}

static int	camera_ops(t_case *c, int at)
{
	t_ray	r;
	t_ray	l;
	t_ray	rr;

	if (cs_is(c->tok[0], "cam_len") || cs_is(c->tok[0], "cam_dir")
		|| cs_is(c->tok[0], "cam_sx") || cs_is(c->tok[0], "cam_sy"))
		r = camera_ray(atoi(c->tok[1]), atoi(c->tok[2]));
	if (cs_is(c->tok[0], "cam_len"))
		return (check_num(vec3_length(r.direction), c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "cam_dir"))
		return (check_vec(r.direction.x, r.direction.y, r.direction.z,
				c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "cam_sx"))
		return (sign_check(r.direction.x, c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "cam_sy"))
		return (sign_check(r.direction.y, c->tok[at], c->raw), 1);
	if (cs_is(c->tok[0], "cam_symmetry"))
	{
		l = camera_ray(0, WIN_HEIGHT / 2);
		rr = camera_ray(WIN_WIDTH - 1, WIN_HEIGHT / 2);
		if (fabs(l.direction.x + rr.direction.x) < 0.01)
			T_OK(c->raw);
		else
			T_NG(c->raw, "화면 좌우 끝의 dir.x 가 대칭이 아님");
		return (1);
	}
	if (cs_is(c->tok[0], "cam_aspect"))
	{
		rr = camera_ray(WIN_WIDTH - 1, WIN_HEIGHT / 2);
		l = camera_ray(WIN_WIDTH / 2, 0);
		if (fabs(rr.direction.x) > fabs(l.direction.y))
			T_OK(c->raw);
		else
			T_NG(c->raw, "가로 화각이 세로보다 넓지 않음 (aspect 누락?)");
		return (1);
	}
	return (0);
}

static int	handle(t_case *c)
{
	t_vec3	r;
	int		at;

	at = cs_arrow(c);
	if (at < 0)
		return (0);
	read_tol(c, at);
	if (cs_is(c->tok[0], "add"))
		r = vec3_add(vec(c->tok[1]), vec(c->tok[2]));
	else if (cs_is(c->tok[0], "sub"))
		r = vec3_sub(vec(c->tok[1]), vec(c->tok[2]));
	else if (cs_is(c->tok[0], "scale"))
		r = vec3_scale(vec(c->tok[1]), cs_num(c->tok[2]));
	else if (cs_is(c->tok[0], "cross"))
		r = vec3_cross(vec(c->tok[1]), vec(c->tok[2]));
	else if (cs_is(c->tok[0], "normalize"))
		r = vec3_normalize(vec(c->tok[1]));
	else if (cs_is(c->tok[0], "ray_at"))
	{
		t_ray	ray;

		ray.origin = vec(c->tok[1]);
		ray.direction = vec(c->tok[2]);
		r = ray_at(ray, cs_num(c->tok[3]));
	}
	else if (cs_is(c->tok[0], "dot"))
		return (check_num(vec3_dot(vec(c->tok[1]), vec(c->tok[2])),
				c->tok[at], c->raw), 1);
	else if (cs_is(c->tok[0], "length"))
		return (check_num(vec3_length(vec(c->tok[1])), c->tok[at], c->raw), 1);
	else if (cs_is(c->tok[0], "norm_len"))
		return (check_num(vec3_length(vec3_normalize(vec(c->tok[1]))),
				c->tok[at], c->raw), 1);
	else
		return (camera_ops(c, at));
	return (check_vec(r.x, r.y, r.z, c->tok[at], c->raw), 1);
}
