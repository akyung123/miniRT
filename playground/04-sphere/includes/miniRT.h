/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
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

// 교차로 인정하는 가장 가까운 t. 0이 아닌 이유는 6단계 그림자 여드름(acne)
# define T_MIN 1e-4
// '무한대' 대신 쓰는 아주 먼 거리
# define T_MAX 1e30

/* ========================================================================== */
/*  팀 공용 타입 - 루트 includes/miniRT.h 와 필드 이름/순서를 똑같이 유지        */
/*  9단계 테스트가 진짜 파서를 이 헤더로 링크하므로, 다르면 메모리가 어긋난다     */
/* ========================================================================== */

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef t_vec3	t_color;

typedef struct s_ambient
{
	double	ratio;
	t_color	color;
}	t_ambient;

typedef struct s_camera
{
	t_vec3	position;
	t_vec3	orientation;
	double	fov;
}	t_camera;

typedef struct s_light
{
	t_vec3	position;
	double	brightness;
	t_color	color;
}	t_light;

typedef enum e_object_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}	t_object_type;

// 반지름이 아니라 '지름'. hit_sphere에 넘길 때 반드시 / 2.0
typedef struct s_sphere
{
	t_vec3	center;
	double	diameter;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;
	double	diameter;
	double	height;
	t_color	color;
}	t_cylinder;

// 도형 셋 중 하나만 담는 공용체. 어떤 게 들었는지는 t_object.type 으로 안다
typedef union u_object_data
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
}	t_object_data;

// 도형 연결 리스트의 노드
typedef struct s_object
{
	t_object_type	type;
	t_object_data	data;
	struct s_object	*next;
}	t_object;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_object	*objects;
}	t_scene;

// mlx에 필요한 것들을 가지고 다니는 구조체
typedef struct s_mlx
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*addr;
	int		bpp;
	int		size_line;
	int		endian;
}	t_mlx;

// miniRT에 필요한 모든 걸 가지고 다닐 구조체 (포인터가 아니라 값으로 들고 있음)
typedef struct s_minirt
{
	t_mlx	mlx;
	t_scene	scene;
}	t_minirt;

/* ========================================================================== */
/*  렌더러 내부 전용 - 팀 헤더에는 넣지 않는다                                   */
/* ========================================================================== */

// 눈은 원점, 화면(창문)은 z = -1

// 광선(ray): P(t) = origin + t * direction
// direction이 단위벡터일 때만 t가 곧 '거리'가 된다
// 나중에 거리를 이용해서 color를 계산할 때, t를 사용하기위해 여기서 direction은 단위벡터로 사용할 것임
typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

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

/* sphere.c */
double	hit_sphere(t_ray r, t_vec3 center, double radius,
			double t_min, double t_max);

/* scene.c */
void	demo_scene(t_scene *scene);

/* rander.c */
t_color	render_pixel(t_scene *scene, int x, int y);
void	rander(t_minirt *mini);

#endif
