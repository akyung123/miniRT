/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <2004924@donga.ac.kr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 20:37:32 by akkim             #+#    #+#             */
/*   Updated: 2025/04/01 21:02:25 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*str;
	size_t	x;

	x = size * nmemb;
	if (__builtin_expect(size && ((x / size) != nmemb), 0))
		return (NULL);
	str = malloc(nmemb * size);
	if (!str)
		return ((void *)0);
	ft_bzero (str, nmemb * size);
	return (str);
}
