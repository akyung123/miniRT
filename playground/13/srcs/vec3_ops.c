#include "miniRT.h"

t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	return (r);
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	return (r);
}

t_vec3	vec3_scale(t_vec3 v, double t)
{
	t_vec3	r;

	r.x = v.x * t;
	r.y = v.y * t;
	r.z = v.z * t;
	return (r);
}

t_vec3	vec3_negate(t_vec3 v)
{
	return (vec3_scale(v, -1.0));
}

t_vec3	vec3_mul(t_vec3 a, t_vec3 b)
{
	t_vec3	r;

	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;
	return (r);
}
