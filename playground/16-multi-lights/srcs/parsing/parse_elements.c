/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdlib.h>

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
	if (!normalize_direction(&scene->camera.orientation))
		return (0);
	scene->camera.fov = str_to_double(tokens[3], &ok);
	if (!ok || scene->camera.fov < 0.0 || scene->camera.fov > 180.0)
		return (0);
	flags->has_camera = 1;
	return (1);
}

static void	add_light(t_scene *scene, t_light *light)
{
	t_light	*cur;

	light->next = NULL;
	if (!scene->lights)
	{
		scene->lights = light;
		return ;
	}
	cur = scene->lights;
	while (cur->next)
		cur = cur->next;
	cur->next = light;
}

int	parse_light(char **tokens, t_scene *scene, t_parse_flags *flags)
{
	t_light	*light;
	int		ok;

	if (count_tokens(tokens) != 4)
		return (0);
	light = malloc(sizeof(t_light));
	if (!light)
		return (0);
	if (!parse_vec3(tokens[1], &light->position))
		return (free(light), 0);
	light->brightness = str_to_double(tokens[2], &ok);
	if (!ok || light->brightness < 0.0 || light->brightness > 1.0)
		return (free(light), 0);
	if (!parse_color(tokens[3], &light->color))
		return (free(light), 0);
	add_light(scene, light);
	flags->has_light = 1;
	return (1);
}
