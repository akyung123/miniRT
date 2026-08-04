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

int	parse_sphere(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (count_tokens(tokens) != 4)
		return (0);
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (0);
	obj->type = OBJ_SPHERE;
	if (!parse_vec3(tokens[1], &obj->data.sphere.center))
		return (free(obj), 0);
	obj->data.sphere.diameter = str_to_double(tokens[2], &ok);
	if (!ok || obj->data.sphere.diameter <= 0.0)
		return (free(obj), 0);
	if (!parse_color(tokens[3], &obj->data.sphere.color))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_plane(char **tokens, t_scene *scene)
{
	t_object	*obj;

	if (count_tokens(tokens) != 4)
		return (0);
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (0);
	obj->type = OBJ_PLANE;
	if (!parse_vec3(tokens[1], &obj->data.plane.point))
		return (free(obj), 0);
	if (!parse_vec3(tokens[2], &obj->data.plane.normal))
		return (free(obj), 0);
	if (!valid_direction(&obj->data.plane.normal))
		return (free(obj), 0);
	if (!parse_color(tokens[3], &obj->data.plane.color))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}

int	parse_cylinder(char **tokens, t_scene *scene)
{
	t_object	*obj;
	int			ok;

	if (count_tokens(tokens) != 6)
		return (0);
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (0);
	obj->type = OBJ_CYLINDER;
	if (!parse_vec3(tokens[1], &obj->data.cylinder.center))
		return (free(obj), 0);
	if (!parse_vec3(tokens[2], &obj->data.cylinder.axis))
		return (free(obj), 0);
	if (!valid_direction(&obj->data.cylinder.axis))
		return (free(obj), 0);
	obj->data.cylinder.diameter = str_to_double(tokens[3], &ok);
	if (!ok || obj->data.cylinder.diameter <= 0.0)
		return (free(obj), 0);
	obj->data.cylinder.height = str_to_double(tokens[4], &ok);
	if (!ok || obj->data.cylinder.height <= 0.0)
		return (free(obj), 0);
	if (!parse_color(tokens[5], &obj->data.cylinder.color))
		return (free(obj), 0);
	add_object(scene, obj);
	return (1);
}
