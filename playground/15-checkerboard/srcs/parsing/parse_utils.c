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

int	parse_checker(char **tokens, int i, t_object *obj)
{
	int	ok;
	int	n;

	obj->checker = 0;
	n = count_tokens(tokens);
	if (n == i)
		return (1);
	if (n != i + 3)
		return (0);
	if (ft_strcmp(tokens[i], "ck") != 0)
		return (0);
	obj->checker_scale = str_to_double(tokens[i + 1], &ok);
	if (!ok || obj->checker_scale <= 0.0)
		return (0);
	if (!parse_color(tokens[i + 2], &obj->checker_color))
		return (0);
	obj->checker = 1;
	return (1);
}
