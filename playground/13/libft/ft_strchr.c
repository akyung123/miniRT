/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <2004924@donga.ac.kr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:39:21 by akkim             #+#    #+#             */
/*   Updated: 2025/04/01 13:45:30 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *str, int ch)
{
	char	*p;

	p = (char *)str;
	while (*p)
	{
		if (*p == (unsigned char)ch)
			return (p);
		p++;
	}
	if ((unsigned char)ch == '\0')
		return (p);
	return (NULL);
}
