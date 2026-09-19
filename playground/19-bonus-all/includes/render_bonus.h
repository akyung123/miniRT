/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_BONUS_H
# define RENDER_BONUS_H

# include <math.h>
# include "miniRT_bonus.h"

# define T_MIN 1e-4
# define T_MAX 1e30

/* 보너스: Phong 스펙큘러 세기와 하이라이트 날카로움 */
# define SPECULAR_KS 0.4
# define SPECULAR_SHININESS 32.0

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_range
{
	double	min;
	double	max;
}	t_range;

typedef struct s_hit
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_color		color;
	t_object	*obj;
}	t_hit;

/* 보너스: 체크무늬/범프가 공통으로 쓰는 표면 좌표 */
typedef struct s_uv
{
	double	u;
	double	v;
}	t_uv;

/* 보너스: 범프가 법선을 흔들 때 쓰는 접선 기저 */
typedef struct s_tb
{
	t_vec3	t;
	t_vec3	b;
}	t_tb;

typedef struct s_cy
{
	t_vec3	center;
	t_vec3	axis;
	double	radius;
	double	half_h;
	t_range	range;
}	t_cy;

typedef struct s_co
{
	t_vec3	apex;
	t_vec3	axis;
	double	radius;
	double	height;
	double	cos2;
	t_range	range;
}	t_co;

typedef struct s_view
{
	t_camera	cam;
	t_vec3		forward;
	t_vec3		right;
	t_vec3		up;
	double		half_w;
	double		half_h;
}	t_view;

t_vec3	ray_at(t_ray r, double t);
t_view	*camera_view(t_camera *cam);
t_ray	camera_ray(t_camera *cam, int x, int y);
double	hit_sphere(t_ray r, t_sphere *sp, t_range range);
double	hit_plane(t_ray r, t_plane *pl, t_range range);
double	hit_cylinder(t_ray r, t_cy *cy);
double	hit_cone(t_ray r, t_co *co);
t_vec3	sphere_normal(t_vec3 center, double radius, t_vec3 p);
t_vec3	cylinder_normal(t_cylinder *cy, t_vec3 p);
t_vec3	cone_normal(t_cone *cn, t_vec3 p);
int		hit_object(t_object *obj, t_ray r, double t_max, t_hit *rec);
int		hit_scene(t_scene *scene, t_ray r, double t_max, t_hit *rec);
double	diffuse_factor(t_vec3 normal, t_vec3 light_dir);
t_color	apply_ambient(t_color obj, t_color amb, double ratio);
t_color	lighting(t_scene *scene, t_hit *rec, t_vec3 view_dir);
int		in_shadow(t_scene *scene, t_vec3 p, t_vec3 light_pos);
t_vec3	stable_helper(t_vec3 n);
t_uv	object_uv(t_object *obj, t_vec3 p);
t_tb	object_tb(t_object *obj, t_hit *rec);
t_color	apply_checker(t_object *obj, t_hit *rec);
void	apply_bump(t_object *obj, t_hit *rec);

#endif
