/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rander.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

//color 함수
// t_color를 받아서 int를 반환하도로 수정해야함
int	create_argb(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(color.x * 255.0);
	g = (int)(color.y * 255.0);
	b = (int)(color.z * 255.0);

	return ((r << 16) | (g << 8) | b);
}

void    my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
    char    *dst;

    // 색칠할 점이 이미지 크기 안에 있는지 확인하기
    if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
    {
        dst = mlx->addr + (y * mlx->size_line + x * (mlx->bpp / 8));
        *(unsigned int *)dst = color;
    }
}

t_color	color_lerp(t_color a, t_color b, double t)
{
	t_color	out;
	out.x = a.x * (1.0 - t) + b.x * t;
	out.y = a.y * (1.0 - t) + b.y * t;
	out.z = a.z * (1.0 - t) + b.z * t;
	return (out);
}

// 아무것도 안 맞았을 때의 배경 = 3단계 하늘 그라디언트
// 광선이 위를 볼수록(direction.y → +1) 하늘색, 아래를 볼수록 흰색
static t_color	sky(t_ray r)
{
	double	t;

	t = 0.5 * (r.direction.y + 1.0);
	return (color_lerp((t_color){1, 1, 1}, (t_color){0.5, 0.7, 1.0}, t));
}

/*
구 표면점 P의 색 (5단계)

1) P = 광선 위 t 지점
2) n = 구의 법선
3) 법선 뒤집기: 광선이 구 '안쪽'에서 표면에 닿으면 n이 광선과 같은 쪽을 본다
   dot(dir, n) > 0 이면 둘이 같은 방향 → -n 으로 뒤집어 항상 광선 쪽을 보게 한다
   (카메라를 구 안에 넣으면 이게 없을 때 안쪽 벽이 거꾸로 빛난다)
4) SHOW_NORMAL 이 1이면 조명 대신 법선을 색으로 (무지개 구)
   n의 각 성분은 -1~1 → (n + 1) * 0.5 로 0~1 색 범위에 맞춘다
   x → 빨강, y → 초록, z → 파랑. 색이 매끄럽게 변하지 않으면 법선이 틀린 것
5) 아니면 ambient + diffuse, 그리고 0~1로 자르기
*/
static t_color	shade_sphere(t_scene *scene, t_ray r, double t, t_sphere *sp)
{
	t_vec3	p;
	t_vec3	n;

	p = ray_at(r, t);
	n = sphere_normal(sp->center, sp->diameter / 2.0, p);
	if (vec3_dot(r.direction, n) > 0.0)
		n = vec3_negate(n);
	if (SHOW_NORMAL)
		return (vec3_scale(vec3_add(n, (t_vec3){1, 1, 1}), 0.5));
	return (vec3_clamp(lighting(scene, p, n, sp->color), 0.0, 1.0));
}

/*
픽셀 하나의 색

4단계와 같은 closest 순회로 '가장 가까운 구'를 찾는다
달라진 점: 색을 바로 고르지 않고 '어떤 구를 몇 t에서 맞았나'만 기억해 뒀다가
마지막에 그 구 하나만 조명 계산한다 (뒤에 가려질 구까지 계산하면 낭비)
*/
t_color	render_pixel(t_scene *scene, int x, int y)
{
	t_ray		r;
	t_object	*obj;
	t_object	*nearest;
	double		closest;
	double		t;

	r = camera_ray(x, y);
	closest = T_MAX;
	nearest = NULL;
	obj = scene->objects;
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
		{
			t = hit_sphere(r, obj->data.sphere.center,
					obj->data.sphere.diameter / 2.0, T_MIN, closest);
			if (t > 0.0)
			{
				closest = t;
				nearest = obj;
			}
		}
		obj = obj->next;
	}
	if (!nearest)
		return (sky(r));
	return (shade_sphere(scene, r, closest, &nearest->data.sphere));
}

void rander(t_minirt *mini)
{
    unsigned int    color;

    int x, y;
    x = 0;
    y = 0;
    while (x < WIN_WIDTH)
    {
        y = 0;
        while(y < WIN_HEIGHT)
        {
            // create_argb가 이미 0x00RRGGBB 로 만들어주므로 mlx 변환 함수가 필요없다
            // (mlx 함수를 안 부르면 테스트 러너가 mlx 없이 링크할 수 있다)
            // t_minirt가 구조체를 '값'으로 들고 있으니 주소(&)를 넘긴다
            color = create_argb(render_pixel(&mini->scene, x, y));
            my_mlx_pixel_put(&mini->mlx, x, y, color);
            y++;
        }
        x++;
    }
}

// 구성을 어떻게 하지?
// 렌더->렌더 픽셀(color를 반환함)->색상 변환->점 찍기
