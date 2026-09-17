/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:13:41 by akkim             #+#    #+#             */
/*   Updated: 2025/12/13 17:28:11 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_init_printf_info(t_info *info)
{
	info->perc = 0;
	info->width = 0;
	info->alt = 0;
	info->spacd = 0;
	info->left = 0;
	info->showsign = 0;
	info->pad = 1;
	info->dot = 0;
}

void	ft_exit_printf(const char **format, t_info *info)
{
	while ((**format == ' ' || **format == '#' || **format == '-' )
		|| (**format == '+' || **format == '0'))
	{
		if (**format == ' ')
			info->spacd = 1;
		else if (**format == '#')
			info->alt = 1;
		else if (**format == '-')
			info->left = 1;
		else if (**format == '+')
			info->showsign = 1;
		else if (**format == '0')
			info->pad = 0;
		(*format)++;
	}
	if (info->left)
		info->pad = 1;
	if (info->showsign)
		info->spacd = 0;
}

const char	*ft_parse_printf_format(const char *format, t_info *info)
{
	ft_init_printf_info(info);
	ft_exit_printf(&format, info);
	if (ft_isdigit(*format))
	{
		info->width = ft_atoi(format);
		while (ft_isdigit(*format))
			format++;
	}
	if (*format == '.')
	{
		format++;
		info->dot = 1;
		if (ft_isdigit(*format))
			info->perc = ft_atoi(format);
		while (ft_isdigit(*format))
			format++;
	}
	else
		info->perc = 0;
	if (info->dot)
		info->pad = 1;
	info->spec = *format++;
	return (format);
}
