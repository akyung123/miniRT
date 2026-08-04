/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:13:36 by akkim             #+#    #+#             */
/*   Updated: 2025/12/13 17:28:13 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_handle(va_list arg, t_info *info)
{
	int	count;

	count = 0;
	if (info->spec == 'c')
		count = ft_printf_char((char)va_arg(arg, int), info);
	else if (info->spec == 's')
		count = ft_printf_string(va_arg(arg, char *), info);
	else if (info->spec == 'p')
		count = ft_printf_pointer(va_arg(arg, void *), info);
	else if (info->spec == 'd' || info->spec == 'i')
		count = ft_printf_int(va_arg(arg, int), info);
	else if (info->spec == 'u')
		count = ft_printf_unit(va_arg(arg, unsigned int), info);
	else if (info->spec == 'x' || info->spec == 'X')
		count = ft_printf_hex(va_arg(arg, unsigned int), info);
	else if (info->spec == '%')
	{
		count++;
		ft_putstr_fd("%", 1);
	}
	return (count);
}
