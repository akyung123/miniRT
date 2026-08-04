/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <2004924@donga.ac.kr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:52:13 by akkim             #+#    #+#             */
/*   Updated: 2025/04/07 18:38:22 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	num_len(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
		return (11);
	if (n <= 0)
		len++;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	negative(int *n, char *num)
{
	if (*n == -2147483648)
	{
		num[0] = '-';
		num[1] = '2';
		*n = 147483648;
	}
	else if (*n < 0)
	{
		num[0] = '-';
		*n = -*n;
	}
}

char	*ft_itoa(int n)
{
	char	*num;
	int		len;

	len = num_len(n);
	num = (char *)malloc((len + 1) * sizeof(char));
	if (!num)
		return (NULL);
	if (n == 0)
		num[0] = '0';
	negative(&n, num);
	num[len] = '\0';
	while (n > 0)
	{
		num[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (num);
}
