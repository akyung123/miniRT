/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by seoykim          ###   ########.fr       */
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
