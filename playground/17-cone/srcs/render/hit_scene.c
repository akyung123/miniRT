/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

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

static int	hit_cylinder_obj(t_cylinder *cy, t_ray r, double t_max,
		t_hit *rec)
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

static void	fill_cone(t_cone *cn, t_range range, t_co *c)
{
	double	h;
	double	rad;

	h = cn->height;
	rad = cn->diameter / 2.0;
	c->apex = vec3_add(cn->center, vec3_scale(cn->axis, h / 2.0));
	c->axis = vec3_negate(cn->axis);
	c->radius = rad;
	c->height = h;
	c->cos2 = (h * h) / (h * h + rad * rad);
	c->range = range;
}

static int	hit_cone_obj(t_cone *cn, t_ray r, double t_max, t_hit *rec)
{
	t_co	c;
	t_range	range;
	double	t;

	range.min = T_MIN;
	range.max = t_max;
	fill_cone(cn, range, &c);
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
	if (obj->type == OBJ_SPHERE)
		return (hit_sphere_obj(&obj->data.sphere, r, t_max, rec));
	if (obj->type == OBJ_PLANE)
		return (hit_plane_obj(&obj->data.plane, r, t_max, rec));
	if (obj->type == OBJ_CYLINDER)
		return (hit_cylinder_obj(&obj->data.cylinder, r, t_max, rec));
	if (obj->type == OBJ_CONE)
		return (hit_cone_obj(&obj->data.cone, r, t_max, rec));
	return (0);
}

int	hit_scene(t_scene *scene, t_ray r, double t_max, t_hit *rec)
{
	t_object	*obj;
	int			found;

	found = 0;
	obj = scene->objects;
	while (obj)
	{
		if (hit_object(obj, r, t_max, rec))
		{
			found = 1;
			t_max = rec->t;
		}
		obj = obj->next;
	}
	if (found && vec3_dot(r.direction, rec->normal) > 0.0)
		rec->normal = vec3_negate(rec->normal);
	return (found);
}
