/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:20:53 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/20 15:20:55 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"

void	free_scene(t_scene *scene)
{
	t_object	*cur;
	t_object	*next;

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
}
