/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_unit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:13:41 by akkim             #+#    #+#             */
/*   Updated: 2025/12/13 17:28:42 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_unintlen(unsigned int num)
{
	int	l;

	l = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= 10;
		l++;
	}
	return (l);
}

int	ft_printf_u_perc(unsigned int n, int perc, t_info *info)
{
	int	count;

	count = 0;
	if (perc > 0)
		count = perc;
	while (perc-- > 0)
		write(1, "0", 1);
	if (info->dot == 1 && n == 0)
		return (count);
	ft_putunbr_fd(n, 1);
	count += ft_unintlen(n);
	return (count);
}

int	ft_printf_u_width(int width, int pad)
{
	int	count;

	if (width < 0)
		return (0);
	count = width;
	if (pad == 0)
		while (width-- > 0)
			write(1, "0", 1);
	else
		while (width-- > 0)
			write(1, " ", 1);
	return (count);
}

int	ft_printf_unit(unsigned int n, t_info *info)
{
	int	count;
	int	len;
	int	l;

	count = 0;
	len = ft_unintlen(n);
	if (n == 0 && info->dot == 1)
		len = 0;
	l = info->width - info->perc;
	if (len > info->perc)
		l = info->width - len;
	if (info->left == 1)
		count += ft_printf_u_perc(n, info->perc - len, info);
	count += ft_printf_u_width(l, info->pad);
	if (info->left == 0)
		count += ft_printf_u_perc(n, info->perc - len, info);
	return (count);
}
