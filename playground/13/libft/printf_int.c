/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:13:41 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 02:24:45 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_intlen(int num)
{
	int	l;

	l = 0;
	if (num == 0)
		return (1);
	if (num == -2147483648)
		return (11);
	if (num < 0)
	{
		l++;
		num *= -1;
	}
	while (num > 0)
	{
		num /= 10;
		l++;
	}
	return (l);
}

int	ft_printf_int_width(int width, int pad)
{
	int	count;

	count = width;
	if (width < 0)
		return (0);
	if (pad == 0)
		while (width-- > 0)
			write(1, "0", 1);
	else
		while (width-- > 0)
			write(1, " ", 1);
	return (count);
}

int	ft_printf_int(int n, t_info *info)
{
	int	count;
	int	len;
	int	l;

	count = 0;
	len = ft_intlen(n);
	if (n == 0 && info->dot == 1)
		len = 0;
	if (n > 0 && (info->showsign == 1 || info->spacd == 1))
		len++;
	l = info->width - info->perc;
	if (len > info->perc)
		l = info->width - len;
	if (n < 0 && info->perc >= len)
		l--;
	if (n < 0)
		len = ft_intlen(n) - 1;
	if (info->left == 1)
		count += ft_printf_perc(n, info, info->perc - len);
	if (info->pad == 0)
		count += ft_printf_pad(n, info);
	count += ft_printf_int_width(l, info->pad);
	if (info->left == 0)
		count += ft_printf_perc(n, info, info->perc - len);
	return (count);
}
