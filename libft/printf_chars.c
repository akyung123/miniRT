/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_chars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:15:34 by akkim             #+#    #+#             */
/*   Updated: 2025/12/13 17:28:30 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_printf_perc(int n, t_info *info, int perc)
{
	int	count;

	count = 0;
	if (perc > 0)
		count = perc;
	if (info->pad != 0)
		count += ft_printf_pad(n, info);
	while (perc-- > 0)
		write(1, "0", 1);
	if (info->dot == 1 && n == 0)
		return (count);
	ft_putnbr_fd(n, 1);
	count += ft_intlen(n);
	return (count);
}

int	ft_printf_width(int width, int pad)
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

int	ft_printf_pad(int n, t_info *info)
{
	if (n < 0)
		return (write(1, "-", 1), 0);
	else if (n >= 0 && (info->spacd == 1 || info->showsign == 1))
	{
		if (info->showsign == 1)
			return (write(1, "+", 1), 1);
		else if (info->spacd == 1)
			return (write(1, " ", 1), 1);
	}
	return (0);
}

int	ft_printf_char(char spce, t_info *info)
{
	int	count;

	count = 0;
	if (info->left == 1)
		ft_putchar_fd(spce, 1);
	count += ft_printf_width(info->width - 1, 1);
	if (info->left == 0)
		ft_putchar_fd(spce, 1);
	return (count + 1);
}

int	ft_printf_string(char *str, t_info *info)
{
	int		count;
	size_t	len;

	count = 0;
	if (!str)
		if (info->dot == 0 || info->perc >= 6)
			str = "(null)";
	len = ft_strlen(str);
	if (info->dot == 1 && info->perc < (int)len)
		len = info->perc;
	if (info->left == 1)
		write(1, str, len);
	count += ft_printf_width(info->width - len, info->pad);
	if (info->left == 0)
		write(1, str, len);
	count += len;
	return (count);
}
