/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "libft.h"
#include "parsing.h"

int	parse_vec3(const char *str, t_vec3 *out)
{
	char	**fields;
	int		ok;

	fields = ft_split(str, ',');
	if (!fields || count_tokens(fields) != 3)
		return (free_tokens(fields), 0);
	out->x = str_to_double(fields[0], &ok);
	if (ok)
		out->y = str_to_double(fields[1], &ok);
	if (ok)
		out->z = str_to_double(fields[2], &ok);
	free_tokens(fields);
	return (ok);
}

static int	parse_rgb(char **fields, double *rgb)
{
	int	i;
	int	ok;

	i = 0;
	ok = 1;
	while (ok && i < 3)
	{
		if (ft_strchr(fields[i], '.'))
			ok = 0;
		if (ok)
			rgb[i] = str_to_double(fields[i], &ok);
		if (ok && (rgb[i] < 0.0 || rgb[i] > 255.0))
			ok = 0;
		i++;
	}
	return (ok);
}

int	parse_color(const char *str, t_color *out)
{
	char	**fields;
	double	rgb[3];

	fields = ft_split(str, ',');
	if (!fields || count_tokens(fields) != 3)
		return (free_tokens(fields), 0);
	if (!parse_rgb(fields, rgb))
		return (free_tokens(fields), 0);
	free_tokens(fields);
	out->x = rgb[0] / 255.0;
	out->y = rgb[1] / 255.0;
	out->z = rgb[2] / 255.0;
	return (1);
}

int	normalize_direction(t_vec3 *v)
{
	double	mag;

	if (v->x < -1.0 || v->x > 1.0)
		return (0);
	if (v->y < -1.0 || v->y > 1.0)
		return (0);
	if (v->z < -1.0 || v->z > 1.0)
		return (0);
	mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	if (mag < 1e-6)
		return (0);
	v->x /= mag;
	v->y /= mag;
	v->z /= mag;
	return (1);
}
