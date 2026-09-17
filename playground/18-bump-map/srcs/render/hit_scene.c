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
	if (hit)
		rec->obj = obj;
	return (hit);
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
	if (found && rec->obj && rec->obj->bump)
		apply_bump(rec->obj, rec);
	return (found);
}
