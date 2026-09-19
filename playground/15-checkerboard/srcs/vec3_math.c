#include <math.h>
#include "miniRT.h"

double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	r;

	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;
	return (r);
}

double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_dot(v, v)));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len < 1e-9)
		return (v);
	return (vec3_scale(v, 1.0 / len));
}

t_vec3	vec3_clamp(t_vec3 v, double min, double max)
{
	t_vec3	r;

	r = v;
	if (r.x < min)
		r.x = min;
	if (r.x > max)
		r.x = max;
	if (r.y < min)
		r.y = min;
	if (r.y > max)
		r.y = max;
	if (r.z < min)
		r.z = min;
	if (r.z > max)
		r.z = max;
	return (r);
}
