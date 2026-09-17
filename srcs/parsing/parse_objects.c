#include <stdlib.h>
#include "parsing.h"

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

static t_object	*new_object(t_object_type type)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = type;
	obj->checker = 0;
	return (obj);
}

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 7)
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
		|| !parse_checker(tokens, 4, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_object	*obj;

	if (count_tokens(tokens) != 4 && count_tokens(tokens) != 7)
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
		|| !parse_checker(tokens, 4, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (count_tokens(tokens) != 6 && count_tokens(tokens) != 9)
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
		|| !parse_checker(tokens, 6, obj))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}
