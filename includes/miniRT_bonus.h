/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

/* ---------- Image resolution (shared by input and rendering) ---------- */

# define WIN_WIDTH 800
# define WIN_HEIGHT 600

/* ---------- 3D data / color ---------- */

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef t_vec3	t_color;

/* ---------- Vector math utils (shared by parser and renderer) ---------- */

t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_scale(t_vec3 v, double t);
t_vec3	vec3_negate(t_vec3 v);
t_vec3	vec3_mul(t_vec3 a, t_vec3 b);
double	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
double	vec3_length(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_clamp(t_vec3 v, double min, double max);

/* ---------- Scene elements (filled by the parser) ---------- */

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

/* 보너스: 광원을 연결 리스트로 만들어 L 줄을 여러 번 쓸 수 있게 한다. */
typedef struct s_light
{
	t_vec3			position;
	double			brightness;
	t_color			color;
	struct s_light	*next;
}	t_light;

typedef enum e_object_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER,
	OBJ_CONE
}	t_object_type;

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

/* 보너스: 2차 곡면 하나 더 (원뿔). 필드 구성은 실린더와 같고,
 * center 는 밑면과 꼭짓점의 중간(축 방향 중심)이다.
 */
typedef struct s_cone
{
	t_vec3	center;
	t_vec3	axis;
	double	diameter;
	double	height;
	t_color	color;
}	t_cone;

typedef union u_object_data
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
	t_cone		cone;
}	t_object_data;

/* 보너스: 체크무늬(ck)와 절차적 범프(bp)는 도형 종류와 무관한
 * 표면 속성이라 t_object 공통 자리에 둔다. 씬 파일에서 색 뒤에
 * `ck <칸크기> <색>` / `bp <주기> <세기>` 를 순서 상관없이 붙인다.
 */
typedef struct s_object
{
	t_object_type	type;
	t_object_data	data;
	int				checker;
	double			checker_scale;
	t_color			checker_color;
	int				bump;
	double			bump_freq;
	double			bump_strength;
	struct s_object	*next;
}	t_object;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights;
	t_object	*objects;
}	t_scene;

/* ---------- Parser-internal (validation, not part of t_scene) ---------- */

typedef struct s_parse_flags
{
	int	has_ambient;
	int	has_camera;
	int	has_light;
}	t_parse_flags;

/* ---------- Output (mlx) struct ----------
 * Internal shape/color calculation types (t_ray, t_hit, etc.) are not
 * declared here; they stay local to the renderer's own implementation.
 */

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

/* ---------- Top-level state ---------- */

typedef struct s_minirt
{
	t_mlx	mlx;
	t_scene	scene;
}	t_minirt;

/* ---------- Error handling ---------- */

void	ft_error(const char *msg);
void	ft_sys_error(const char *context);

/* ---------- Parsing interface ---------- */

int		parse_scene(const char *path, t_scene *scene);
void	free_scene(t_scene *scene);

/* ---------- Rendering interface (shape + color calculation) ----------
 * The only boundary between input/mlx/output and the renderer.
 * The renderer builds the ray for pixel (x, y) from scene->camera and
 * WIN_WIDTH/WIN_HEIGHT above (width/height are macros, not parameters,
 * because of the norm's 4-argument limit).
 * Returns t_color (= t_vec3, x/y/z = r/g/b, normalized to 0.0~1.0);
 * the output side converts it to 0~255 and packs it into the mlx format.
 */

t_color	render_pixel(t_scene *scene, int x, int y);

#endif
