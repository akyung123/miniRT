/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing_bonus.h"

void	add_object(t_scene *scene, t_object *obj)
{
	t_object	*cur;

	obj->next = NULL;
	if (!scene->objects)
	{
		scene->objects = obj;
		return ;
	}
	cur = scene->objects;
	while (cur->next)
		cur = cur->next;
	cur->next = obj;
}

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (!count_ok(tokens, 4))
		return (0);
	obj = new_object(OBJ_SPHERE);
	if (!obj)
		return (0);
	if (!parse_vec3(tokens[1], &obj->data.sphere.center))
		return (free(obj), 0);
	obj->data.sphere.diameter = str_to_double(tokens[2], &ok);
	if (!ok || obj->data.sphere.diameter <= 0.0)
		return (free(obj), 0);
	if (!parse_color(tokens[3], &obj->data.sphere.color)
		|| !parse_modifiers(tokens, 4, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_object	*obj;

	if (!count_ok(tokens, 4))
		return (0);
	obj = new_object(OBJ_PLANE);
	if (!obj)
		return (0);
	if (!parse_vec3(tokens[1], &obj->data.plane.point))
		return (free(obj), 0);
	if (!parse_vec3(tokens[2], &obj->data.plane.normal))
		return (free(obj), 0);
	if (!normalize_direction(&obj->data.plane.normal))
		return (free(obj), 0);
	if (!parse_color(tokens[3], &obj->data.plane.color)
		|| !parse_modifiers(tokens, 4, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (!count_ok(tokens, 6))
		return (0);
	obj = new_object(OBJ_CYLINDER);
	if (!obj)
		return (0);
	if (!parse_vec3(tokens[1], &obj->data.cylinder.center))
		return (free(obj), 0);
	if (!parse_vec3(tokens[2], &obj->data.cylinder.axis))
		return (free(obj), 0);
	if (!normalize_direction(&obj->data.cylinder.axis))
		return (free(obj), 0);
	obj->data.cylinder.diameter = str_to_double(tokens[3], &ok);
	if (!ok || obj->data.cylinder.diameter <= 0.0)
		return (free(obj), 0);
	obj->data.cylinder.height = str_to_double(tokens[4], &ok);
	if (!ok || obj->data.cylinder.height <= 0.0)
		return (free(obj), 0);
	if (!parse_color(tokens[5], &obj->data.cylinder.color)
		|| !parse_modifiers(tokens, 6, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_cone(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (!count_ok(tokens, 6))
		return (0);
	obj = new_object(OBJ_CONE);
	if (!obj)
		return (0);
	if (!parse_vec3(tokens[1], &obj->data.cone.center))
		return (free(obj), 0);
	if (!parse_vec3(tokens[2], &obj->data.cone.axis))
		return (free(obj), 0);
	if (!normalize_direction(&obj->data.cone.axis))
		return (free(obj), 0);
	obj->data.cone.diameter = str_to_double(tokens[3], &ok);
	if (!ok || obj->data.cone.diameter <= 0.0)
		return (free(obj), 0);
	obj->data.cone.height = str_to_double(tokens[4], &ok);
	if (!ok || obj->data.cone.height <= 0.0)
		return (free(obj), 0);
	if (!parse_color(tokens[5], &obj->data.cone.color)
		|| !parse_modifiers(tokens, 6, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}
