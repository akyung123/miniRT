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
픽셀 하나의 색 = 그 픽셀로 쏜 광선이 '가장 먼저' 맞는 구의 색

핵심은 closest
  - 처음엔 T_MAX(사실상 무한대)
  - 구를 하나 맞출 때마다 closest = 그 t 로 줄인다
  - 다음 구는 t_max 자리에 closest를 넘겨서 '지금까지 찾은 것보다 가까운 교차'만 받는다
  → 리스트 순서와 상관없이 마지막에 남는 건 가장 가까운 구
  → 겹친 구 중 앞의 것이 그려지는 '가려짐'이 이것만으로 해결된다
*/
t_color	render_pixel(t_scene *scene, int x, int y)
{
	t_ray		r;
	t_object	*obj;
	t_color		color;
	double		closest;
	double		t;

	r = camera_ray(x, y);
	color = sky(r);
	closest = T_MAX;
	obj = scene->objects;
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
		{
			// 구조체는 지름을 들고 있으니 / 2.0
			t = hit_sphere(r, obj->data.sphere.center,
					obj->data.sphere.diameter / 2.0, T_MIN, closest);
			// 음수 = 안 맞음 (또는 이미 찾은 것보다 멀다)
			if (t > 0.0)
			{
				closest = t;
				color = obj->data.sphere.color;
			}
		}
		obj = obj->next;
	}
	return (color);
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
