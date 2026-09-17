/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <2004924@donga.ac.kr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:50:28 by akkim             #+#    #+#             */
/*   Updated: 2025/04/01 14:01:48 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *ptr, int value, size_t num)
{
	size_t			i;
	unsigned char	*p;

	p = (unsigned char *)ptr;
	i = 0;
	while (i < num)
	{
		if (*(p + i) == (unsigned char)value)
			return ((void *)(p + i));
		i++;
	}
	return (NULL);
}
