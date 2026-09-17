#include "parsing.h"

static double	parse_int_part(const char **str)
{
	double	result;

	result = 0.0;
	while (**str >= '0' && **str <= '9')
		result = result * 10.0 + (*(*str)++ - '0');
	return (result);
}

static double	parse_frac_part(const char **str)
{
	double	result;
	double	frac;

	result = 0.0;
	frac = 0.1;
	while (**str >= '0' && **str <= '9')
	{
		result += (*(*str)++ - '0') * frac;
		frac *= 0.1;
	}
	return (result);
}

double	str_to_double(const char *str, int *ok)
{
	double	sign;
	double	result;

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
	result = parse_int_part(&str);
	if (*str == '.')
	{
		str++;
		result += parse_frac_part(&str);
	}
	if (*str != '\0')
		*ok = 0;
	return (sign * result);
}
