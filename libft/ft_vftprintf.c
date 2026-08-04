/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vftprintf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:14:28 by akkim             #+#    #+#             */
/*   Updated: 2025/12/13 17:28:25 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_vfprintf(const char *format, va_list arg)
{
	int		count;
	t_info	info;

	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			format = ft_parse_printf_format(format, &info);
			count += ft_handle(arg, &info);
		}
		else
		{
			ft_putchar_fd(*format, 1);
			format++;
			count++;
		}
	}
	return (count);
}
