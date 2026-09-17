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

# define WIN_WIDTH 800
# define WIN_HEIGHT 600

// 3D vector 구조체
typedef struct s_vec3
{
    double  x;
    double  y;
    double  z;
}   t_vec3;

typedef t_vec3	t_color;
// 추후 교체 및 수정 예정
typedef struct s_scene	t_scene;

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
typedef struct s_minirt
{
    t_mlx   *mlx;
    t_scene *scene;
}   t_minirt;

void	draw(t_minirt *mini);
void    rander(t_minirt *mini);

#endif