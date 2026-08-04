#include "parsing.h"

int	parse_ambient(char **tokens, t_scene *scene, t_parse_flags *flags)
{
	int	ok;

	if (flags->has_ambient || count_tokens(tokens) != 3)
		return (0);
	scene->ambient.ratio = str_to_double(tokens[1], &ok);
	if (!ok || scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (0);
	if (!parse_color(tokens[2], &scene->ambient.color))
		return (0);
	flags->has_ambient = 1;
	return (1);
}

int	parse_camera(char **tokens, t_scene *scene, t_parse_flags *flags)
{
	int	ok;

	if (flags->has_camera || count_tokens(tokens) != 4)
		return (0);
	if (!parse_vec3(tokens[1], &scene->camera.position))
		return (0);
	if (!parse_vec3(tokens[2], &scene->camera.orientation))
		return (0);
	if (!valid_direction(&scene->camera.orientation))
		return (0);
	scene->camera.fov = str_to_double(tokens[3], &ok);
	if (!ok || scene->camera.fov < 0.0 || scene->camera.fov > 180.0)
		return (0);
	flags->has_camera = 1;
	return (1);
}

int	parse_light(char **tokens, t_scene *scene, t_parse_flags *flags)
{
	int	ok;

	if (flags->has_light || count_tokens(tokens) != 4)
		return (0);
	if (!parse_vec3(tokens[1], &scene->light.position))
		return (0);
	scene->light.brightness = str_to_double(tokens[2], &ok);
	if (!ok || scene->light.brightness < 0.0 || scene->light.brightness > 1.0)
		return (0);
	if (!parse_color(tokens[3], &scene->light.color))
		return (0);
	flags->has_light = 1;
	return (1);
}
