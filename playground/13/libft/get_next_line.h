/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:48:24 by marvin            #+#    #+#             */
/*   Updated: 2026/01/19 18:00:48 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include "libft.h"
# include "ft_printf_bonus.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

char	*get_next_line(int fd);
char	*ft_read_line(int fd, char **save, char **buf);
char	*ft_new_save(char **save);
char	*ft_nextstrcpy(char *str1);
char	*ft_befstrcpy(char *str1);

#endif