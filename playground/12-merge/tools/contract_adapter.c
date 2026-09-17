/* 학습 단계 테스트 계약 → srcs 렌더러 함수 - 제출 대상 아님
 * shim 을 거치지 않고 render.h 를 직접 봐야 진짜 hit_sphere 를 부를 수 있다
 */
#include "render.h"

typedef struct s_sphere_lite
{
	t_vec3	center;
	double	radius;
}	t_sphere_lite;

static t_range	range_of(double t_min, double t_max)
{
	t_range	r;

	r.min = t_min;
	r.max = t_max;
	return (r);
}

double	contract_hit_sphere(t_ray r, t_vec3 center, double radius,
			double t_min, double t_max)
{
	t_sphere	sp;

	sp.center = center;
	sp.diameter = radius * 2.0;
	return (hit_sphere(r, &sp, range_of(t_min, t_max)));
}

double	contract_hit_plane(t_ray r, t_vec3 point, t_vec3 normal,
			double t_min, double t_max)
{
	t_plane	pl;

	pl.point = point;
	pl.normal = normal;
	return (hit_plane(r, &pl, range_of(t_min, t_max)));
}

double	contract_hit_cylinder(t_ray r, t_vec3 center, t_vec3 axis,
			double radius, double height, double t_min, double t_max)
{
	t_cy	cy;

	cy.center = center;
	cy.axis = axis;
	cy.radius = radius;
	cy.half_h = height / 2.0;
	cy.range = range_of(t_min, t_max);
	return (hit_cylinder(r, &cy));
}

/* 6단계 계약: 구 배열 중 하나라도 P 와 광원 사이를 막으면 1 (in_shadow 와 같은 판정) */
int	shadow_test(t_vec3 p, t_vec3 light_pos, t_sphere_lite *blockers, int n)
{
	t_ray	r;
	double	dist;
	int		i;

	r.origin = p;
	dist = vec3_length(vec3_sub(light_pos, p));
	r.direction = vec3_normalize(vec3_sub(light_pos, p));
	i = 0;
	while (i < n)
	{
		if (contract_hit_sphere(r, blockers[i].center, blockers[i].radius,
				T_MIN, dist) > 0.0)
			return (1);
		i++;
	}
	return (0);
}
