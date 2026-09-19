/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "parsing.h"

int	count_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

int	parse_bump(char **tokens, int i, t_object *obj)
{
	int	ok;
	int	n;

	obj->bump = 0;
	obj->bump_freq = 1.0;
	obj->bump_strength = 0.0;
	n = count_tokens(tokens);
	if (n == i)
		return (1);
	if (n != i + 3)
		return (0);
	if (ft_strcmp(tokens[i], "bp") != 0)
		return (0);
	obj->bump_freq = str_to_double(tokens[i + 1], &ok);
	if (!ok || obj->bump_freq <= 0.0)
		return (0);
	obj->bump_strength = str_to_double(tokens[i + 2], &ok);
	if (!ok || obj->bump_strength < 0.0)
		return (0);
	obj->bump = 1;
	return (1);
}
