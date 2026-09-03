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

// 렌더러 안에서 색은 언제나 0.0 ~ 1.0 실수. 0~255 로 바꾸는 건 출력 몫이다
typedef struct s_vec3
{
    double  x;
    double  y;
    double  z;
}   t_vec3;

typedef t_vec3  t_color;

// 아직 파서가 없어서 정의는 없다. render_pixel 이 포인터로만 받으므로
// 이름만 있어도 컴파일된다. 4단계에서 본체 정의로 교체
typedef struct s_scene  t_scene;

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
    t_mlx    mlx;
}   t_minirt;

#endif