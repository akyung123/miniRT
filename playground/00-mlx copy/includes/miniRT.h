/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/07/29 16:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "mlx.h"
// # include "libft.h"

# define WIDTH 800
# define HEIGHT 600

// mlx에 필요한 것들을 가지고 다니는 구조체
typedef struct s_mlx
{
    void    *mlx_ptr;
    void    *win_ptr;
    void    *img_ptr;
    char    *addr;
    int     bpp;
    int     size_line;
    int     endian;
}   t_mlx;

// miniRT에 필요한 모든 걸 가지고 다닐 구조체
typedef struct s_mini
{
    t_mlx  *mlx;
}   t_mini;

#endif