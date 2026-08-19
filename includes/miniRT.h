#ifndef MINIRT_H
# define MINIRT_H

/* ---------- 이미지 해상도 (입력/렌더링 공용) ---------- */

# define WIN_WIDTH 800
# define WIN_HEIGHT 600

/* ---------- 3D 데이터 / 색상 ---------- */

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef t_vec3	t_color;

/* ---------- 씬 요소 (파서가 채움) ---------- */

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

typedef union u_object_data
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
}	t_object_data;

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

/* ---------- 파서 내부 전용 (입력 검증용, t_scene에는 안 넣음) ---------- */

typedef struct s_parse_flags
{
	int	has_ambient;
	int	has_camera;
	int	has_light;
}	t_parse_flags;

/* ---------- 출력(mlx) 구조체 ----------
 * t_ray, t_hit 등 형태/색상 계산 내부 타입은 여기 안 넣음 (akkim 구현 내부에서만 사용).
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

/* ---------- 전체 관리 구조체 ---------- */

typedef struct s_minirt
{
	t_mlx	mlx;	// 입력/mlx/출력(나) 담당
	t_scene	scene;	// 파서 -> 렌더러
}	t_minirt;

/* ---------- 에러 처리 ---------- */

void	ft_error(const char *msg);
void	ft_sys_error(const char *context);

/* ---------- 파싱 인터페이스 (다음 단계에서 구현 예정) ---------- */

int		parse_scene(const char *path, t_scene *scene);
void	free_scene(t_scene *scene);

#endif
