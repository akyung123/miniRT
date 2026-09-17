/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pritnf_pointer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 02:13:41 by akkim             #+#    #+#             */
/*   Updated: 2025/12/13 17:28:50 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	ft_printf_to_hex(unsigned long long num, int count)
{
	char	*base;

	base = "0123456789abcdef";
	if (num >= 16)
		count = ft_printf_to_hex(num / 16, 0);
	ft_putchar_fd(base[num % 16], 1);
	count++;
	return (count);
}

int	ft_printf_addr(unsigned long long num)
{
	int	count;
	int	l;

	count = 0;
	if (num == 0)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write(1, "0x", 2);
	l = ft_printf_to_hex(num, count);
	count += l + 2;
	return (count);
}

int	ft_len_addr(void *ptr)
{
	int				len;
	unsigned long	addr;

	addr = (unsigned long)ptr;
	len = 0;
	if (addr == 0)
		return (3);
	while (addr > 0)
	{
		addr >>= 4;
		len++;
	}
	return (len + 2);
}

int	ft_printf_pointer(void *addr, t_info *info)
{
	int					count;
	int					l;
	unsigned long long	num;

	count = 0;
	num = (unsigned long long)addr;
	l = info->width - ft_len_addr(addr);
	if (num == 0)
		l = info->width - 5;
	if (info->left == 1)
		count += ft_printf_addr(num);
	count += ft_printf_width(l, info->pad);
	if (info->left == 0)
		count += ft_printf_addr(num);
	return (count);
}
