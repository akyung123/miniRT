#include <math.h>
#include "parsing.h"

double	str_to_double(const char *str, int *ok)
{
	double	sign;
	double	result;
	double	frac;

	*ok = 1;
	sign = 1.0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1.0;
		str++;
	}
	if (!(*str >= '0' && *str <= '9') && *str != '.')
		*ok = 0;
	result = 0.0;
	while (*str >= '0' && *str <= '9')
		result = result * 10.0 + (*str++ - '0');
	if (*str == '.')
	{
		frac = 0.1;
		str++;
		while (*str >= '0' && *str <= '9')
		{
			result += (*str++ - '0') * frac;
			frac *= 0.1;
		}
	}
	if (*str != '\0')
		*ok = 0;
	return (sign * result);
}

int	parse_vec3(const char *str, t_vec3 *out)
{
	char	**parts;
	int		ok;

	parts = ft_split(str, ',');
	if (!parts || count_tokens(parts) != 3)
		return (free_tokens(parts), 0);
	out->x = str_to_double(parts[0], &ok);
	if (ok)
		out->y = str_to_double(parts[1], &ok);
	if (ok)
		out->z = str_to_double(parts[2], &ok);
	free_tokens(parts);
	return (ok);
}

int	parse_color(const char *str, t_color *out)
{
	char	**parts;
	int		ok;
	double	rgb[3];
	int		i;

	parts = ft_split(str, ',');
	if (!parts || count_tokens(parts) != 3)
		return (free_tokens(parts), 0);
	i = 0;
	ok = 1;
	while (ok && i < 3)
	{
		if (ft_strchr(parts[i], '.'))
			ok = 0;
		if (ok)
			rgb[i] = str_to_double(parts[i], &ok);
		if (ok && (rgb[i] < 0.0 || rgb[i] > 255.0))
			ok = 0;
		i++;
	}
	free_tokens(parts);
	if (!ok)
		return (0);
	out->x = rgb[0] / 255.0;
	out->y = rgb[1] / 255.0;
	out->z = rgb[2] / 255.0;
	return (1);
}

int	valid_direction(t_vec3 *v)
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
