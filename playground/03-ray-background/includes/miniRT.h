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

// 테스트 러너(test_03.c)가 쓰는 이름. 같은 값의 별칭일 뿐
# define WIDTH WIN_WIDTH
# define HEIGHT WIN_HEIGHT

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

// 눈은 z = -1로 둠
// 이유는 그냥 다 그렇게 하길래라서 평가 때, 말할 핑계가 필요함

// 광선(ray): P(t) = origin + t * direction
// direction이 단위벡터일 때만 t가 곧 '거리'가 된다
// 나중에 거리를 이용해서 color를 계산할 때, t를 사용하기위해 여기서 direction은 단위벡터로 사용할 것임
typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction; 
}	t_ray;

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

/* vec3.c */
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_scale(t_vec3 v, double t);
double	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
double	vec3_length(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);

/* ray.c */
t_vec3	ray_at(t_ray r, double t);
t_ray	camera_ray(int x, int y);

void	draw(t_minirt *mini);
void    rander(t_minirt *mini);

#endif
