/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:48:24 by marvin            #+#    #+#             */
/*   Updated: 2026/01/13 16:24:20 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char		*save;
	char			*buf;
	char			*tmp;

	if (fd == -1)
		return (free(save), save = NULL, NULL);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!save || save[0] == '\0')
	{
		free(save);
		save = ft_strjoin("", "");
		if (!save)
			return (NULL);
	}
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(save), NULL);
	tmp = ft_read_line(fd, &save, &buf);
	if (!tmp)
		return (free(buf), NULL);
	if (tmp[0] == '\0')
		return (free(save), free(buf), save = NULL, NULL);
	save = tmp;
	return (free(buf), ft_new_save(&save));
}

char	*ft_read_line(int fd, char **save, char **buf)
{
	ssize_t		read_size;
	char		*new_buf;

	while (!ft_strchr(*save, '\n'))
	{
		read_size = read(fd, *buf, BUFFER_SIZE);
		if (read_size == 0)
			break ;
		else if (read_size < 0)
		{
			free(*save);
			*save = NULL;
			return (NULL);
		}
		(*buf)[read_size] = '\0';
		new_buf = ft_strjoin(*save, *buf);
		free(*save);
		*save = new_buf;
	}
	return (*save);
}

char	*ft_new_save(char **save)
{
	char	*new_line;
	char	*new_save;

	new_line = ft_befstrcpy(*save);
	if (!new_line)
		return (NULL);
	new_save = ft_nextstrcpy(*save);
	free(*save);
	if (new_save && new_save[0] == '\0')
	{
		free(new_save);
		new_save = NULL;
	}
	*save = new_save;
	return (new_line);
}

char	*ft_befstrcpy(char *str1)
{
	int		i;
	int		j;
	char	*str2;

	i = 0;
	j = 0;
	if (!str1)
		return (NULL);
	while (str1[i] && str1[i] != '\n')
		i++;
	if (str1[i] == '\n')
		i++;
	str2 = malloc(i + 1);
	if (!str2)
		return (NULL);
	while (j < i)
	{
		str2[j] = str1[j];
		j++;
	}
	str2[i] = '\0';
	return (str2);
}

char	*ft_nextstrcpy(char *str1)
{
	int		i;
	int		j;
	int		len;
	char	*str2;

	i = 0;
	j = 0;
	len = 0;
	if (!str1)
		return (NULL);
	while (str1[i] && str1[i] != '\n')
		i++;
	if (str1[i] == '\n')
		i++;
	len = ft_strlen(str1 + i);
	str2 = malloc(len + 1);
	if (!str2)
		return (NULL);
	while (j < len)
	{
		str2[j] = str1[i + j];
		j++;
	}
	str2[j] = '\0';
	return (str2);
}
