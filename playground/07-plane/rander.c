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
픽셀 하나의 색 (7단계)

1) 카메라 광선
2) hit_scene: 모든 도형 중 가장 가까운 교차 → rec (t, 점, 법선, 색)
3) 아무것도 안 맞으면 하늘
4) SHOW_NORMAL 이면 법선을 색으로 (-1~1 → 0~1)
5) 아니면 조명(ambient + diffuse + 그림자) 후 0~1로 자르기

5~6단계의 shade_sphere와 구 반복문이 hit_scene 한 줄로 바뀌었다
도형 종류를 아는 곳은 이제 hit.c 하나뿐이다
*/
t_color	render_pixel(t_scene *scene, int x, int y)
{
	t_ray	r;
	t_hit	rec;

	r = camera_ray(x, y);
	if (!hit_scene(scene, r, T_MAX, &rec))
		return (sky(r));
	if (SHOW_NORMAL)
		return (vec3_scale(vec3_add(rec.normal, (t_vec3){1, 1, 1}), 0.5));
	return (vec3_clamp(lighting(scene, &rec), 0.0, 1.0));
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
