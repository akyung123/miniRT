#ifndef MINIRT_H
# define MINIRT_H

/* 본체 includes/miniRT.h 의 부분집합.
 * 2단계에 필요한 것만 남기되, 이름은 한 글자도 바꾸지 않는다.
 * 4단계(카메라)에서 이 파일을 본체 것으로 통째 교체해도
 * .c 파일이 한 줄도 안 바뀌게 하는 것이 목적.
 */

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

/* ---------- 씬 (아직 없음) ----------
 * render_pixel 이 t_scene* 로만 받으므로 정의 없이 이름만 있어도 컴파일된다.
 * 4단계에서 본체의 t_scene 정의로 교체.
 */

typedef struct s_scene	t_scene;

/* ---------- 출력(mlx) 구조체 ---------- */

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

/* ---------- 전체 관리 구조체 ----------
 * 본체는 t_scene 을 값으로 갖는다(t_scene scene). 값으로 가지려면 크기를
 * 알아야 하는데 위에서 정의를 뺐으므로 2단계에서만 포인터로 둔다.
 * 이 파일에서 본체와 다른 곳은 여기 한 줄뿐이다.
 */

typedef struct s_minirt
{
	t_mlx		mlx;
	t_scene		*scene;
}	t_minirt;

#endif
