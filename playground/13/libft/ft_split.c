/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:32:46 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 13:33:23 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(char	const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static int	len_word(char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**free_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		i++;
	}
	free (str);
	return (NULL);
}

static char	**fill_split(char **str, char const *s, char c, int words)
{
	int	i;
	int	j;

	i = 0;
	while (*s && i < words)
	{
		while (*s == c)
			s++;
		str[i] = malloc((len_word((char *)s, c) + 1));
		if (!str[i])
			return (free_split(str));
		j = 0;
		while (*s != c && *s)
			str[i][j++] = *s++;
		str[i][j] = '\0';
		i++;
	}
	str[i] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		words;

	if (!s)
		return (NULL);
	words = count_word(s, c);
	str = malloc((words + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	return (fill_split(str, s, c, words));
}
