/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"

void	free_scene(t_scene *scene)
{
	t_object	*cur;
	t_object	*next;
	t_light		*light;
	t_light		*next_light;

	if (!scene)
		return ;
	cur = scene->objects;
	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	scene->objects = NULL;
	light = scene->lights;
	while (light)
	{
		next_light = light->next;
		free(light);
		light = next_light;
	}
	scene->lights = NULL;
}
