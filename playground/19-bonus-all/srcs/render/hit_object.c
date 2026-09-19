/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

/* 도형 한 개와 광선의 교차. 맞으면 rec 에 t/지점/법선/색을 채운다.
 * 보너스 원뿔(co)이 여기 네 번째 갈래로 들어온다.
 */

static int	hit_sphere_obj(t_sphere *sp, t_ray r, double t_max, t_hit *rec)
{
	t_range	range;
	double	t;

	range.min = T_MIN;
	range.max = t_max;
	t = hit_sphere(r, sp, range);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	rec->normal = sphere_normal(sp->center, sp->diameter / 2.0, rec->point);
	rec->color = sp->color;
	return (1);
}

static int	hit_plane_obj(t_plane *pl, t_ray r, double t_max, t_hit *rec)
{
	t_range	range;
	double	t;

	range.min = T_MIN;
	range.max = t_max;
	t = hit_plane(r, pl, range);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	rec->normal = pl->normal;
	rec->color = pl->color;
	return (1);
}

static int	hit_cylinder_obj(t_cylinder *cy, t_ray r, double t_max, t_hit *rec)
{
	t_cy	c;
	double	t;

	c.center = cy->center;
	c.axis = cy->axis;
	c.radius = cy->diameter / 2.0;
	c.half_h = cy->height / 2.0;
	c.range.min = T_MIN;
	c.range.max = t_max;
	t = hit_cylinder(r, &c);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	rec->normal = cylinder_normal(cy, rec->point);
	rec->color = cy->color;
	return (1);
}

static int	hit_cone_obj(t_cone *cn, t_ray r, double t_max, t_hit *rec)
{
	t_co	c;
	double	t;

	c.height = cn->height;
	c.radius = cn->diameter / 2.0;
	c.apex = vec3_add(cn->center, vec3_scale(cn->axis, c.height / 2.0));
	c.axis = vec3_negate(cn->axis);
	c.cos2 = (c.height * c.height)
		/ (c.height * c.height + c.radius * c.radius);
	c.range.min = T_MIN;
	c.range.max = t_max;
	t = hit_cone(r, &c);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	rec->normal = cone_normal(cn, rec->point);
	rec->color = cn->color;
	return (1);
}

int	hit_object(t_object *obj, t_ray r, double t_max, t_hit *rec)
{
	int	hit;

	hit = 0;
	if (obj->type == OBJ_SPHERE)
		hit = hit_sphere_obj(&obj->data.sphere, r, t_max, rec);
	else if (obj->type == OBJ_PLANE)
		hit = hit_plane_obj(&obj->data.plane, r, t_max, rec);
	else if (obj->type == OBJ_CYLINDER)
		hit = hit_cylinder_obj(&obj->data.cylinder, r, t_max, rec);
	else if (obj->type == OBJ_CONE)
		hit = hit_cone_obj(&obj->data.cone, r, t_max, rec);
	if (hit)
	{
		rec->obj = obj;
		if (obj->checker)
			rec->color = apply_checker(obj, rec);
	}
	return (hit);
}
