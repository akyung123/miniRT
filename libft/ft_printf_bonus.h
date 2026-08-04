/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:12:38 by akkim             #+#    #+#             */
/*   Updated: 2026/01/19 18:00:41 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

# include <stdarg.h>
# include <unistd.h>
# include <limits.h>
# include "libft.h"

typedef struct s_printf_info
{
	int				perc;
	int				width;
	char			spec;
	unsigned int	alt:1;
	unsigned int	spacd:1;
	unsigned int	left:1;
	unsigned int	showsign:1;
	unsigned int	pad:1;
	unsigned int	dot:1;
}		t_info;

int			ft_intlen(int num);
int			ft_printf(const char *format, ...);
int			ft_printf_int(int n, t_info *info);
int			ft_printf_pad(int n, t_info *info);
int			ft_handle(va_list arg, t_info *info);
void		ft_putunbr_fd(unsigned int n, int fd);
int			ft_printf_width(int width, int length);
int			ft_printf_char(char spce, t_info *info);
int			ft_printf_string(char *str, t_info *info);
int			ft_printf_pointer(void *ptr, t_info *info);
int			ft_printf_hex(unsigned int n, t_info *info);
int			ft_printf_unit(unsigned int n, t_info *info);
int			ft_vfprintf(const char *format, va_list arg);
int			ft_printf_perc(int n, t_info *info, int perc);
int			ft_printf_to_hex(unsigned long long num, int count);
const char	*ft_parse_printf_format(const char *format, t_info *info);\

#endif
