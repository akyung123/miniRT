/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
도형별 교차 → t_hit 채우기

hit_sphere / hit_plane 은 't 하나'만 돌려준다 (테스트 계약)
조명에 필요한 건 t, 교차점, 법선, 색이라 여기서 한 번에 뽑아 t_hit에 담는다
이렇게 하면 조명/그림자 코드는 도형 종류를 전혀 몰라도 된다
→ 8단계에서 원기둥을 넣을 때 여기에 함수 하나만 추가하면 끝 (실제로 그렇게 했다)
*/
static int	hit_sphere_obj(t_sphere *sp, t_ray r, double t_max, t_hit *rec)
{
	double	radius;
	double	t;

	radius = sp->diameter / 2.0;
	t = hit_sphere(r, sp->center, radius, T_MIN, t_max);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	rec->normal = sphere_normal(sp->center, radius, rec->point);
	rec->color = sp->color;
	return (1);
}

static int	hit_plane_obj(t_plane *pl, t_ray r, double t_max, t_hit *rec)
{
	double	t;

	t = hit_plane(r, pl->point, pl->normal, T_MIN, t_max);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	// 평면의 법선은 어느 점에서나 같다 (파서가 정규화해서 넣어준다)
	rec->normal = pl->normal;
	rec->color = pl->color;
	return (1);
}

// 8단계: 원기둥 추가. 조명/그림자/render_pixel은 한 줄도 안 바뀐다
static int	hit_cylinder_obj(t_cylinder *cy, t_ray r, double t_max,
		t_hit *rec)
{
	double	t;

	// 구조체는 지름을 들고 있으니 / 2.0, axis는 파서가 정규화해서 넣어준다
	t = hit_cylinder(r, cy->center, cy->axis, cy->diameter / 2.0,
			cy->height, T_MIN, t_max);
	if (t < 0.0)
		return (0);
	rec->t = t;
	rec->point = ray_at(r, t);
	rec->normal = cylinder_normal(cy, rec->point);
	rec->color = cy->color;
	return (1);
}

// type 을 보고 공용체에서 맞는 도형을 꺼내 해당 교차 함수로 보낸다
// 맞았고 t < t_max 일 때만 rec를 덮어쓴다 (안 맞으면 rec는 그대로)
int	hit_object(t_object *obj, t_ray r, double t_max, t_hit *rec)
{
	if (obj->type == OBJ_SPHERE)
		return (hit_sphere_obj(&obj->data.sphere, r, t_max, rec));
	if (obj->type == OBJ_PLANE)
		return (hit_plane_obj(&obj->data.plane, r, t_max, rec));
	if (obj->type == OBJ_CYLINDER)
		return (hit_cylinder_obj(&obj->data.cylinder, r, t_max, rec));
	return (0);
}

/*
씬 전체에서 가장 가까운 교차 찾기

4단계 render_pixel의 closest 순회를 도형 종류와 상관없게 일반화한 것
  - t_max 를 '지금까지 찾은 가장 가까운 t'로 계속 좁힌다
  - 다음 도형은 그보다 가까울 때만 맞은 것으로 치고 rec를 덮어쓴다
  → 반복이 끝나면 rec에는 가장 가까운 교차만 남는다 (임시 변수가 필요 없는 이유)

마지막에 법선 뒤집기 (5단계와 같은 이유, 이제 모든 도형에 적용)
  평면을 아래에서 올려다보거나, 구 안에서 보면 법선이 광선과 같은 쪽을 본다
  dot(d, n) > 0 이면 -n 으로 뒤집어 '광선이 온 쪽'을 보게 한다

맞은 게 있으면 1, 없으면 0
*/
int	hit_scene(t_scene *scene, t_ray r, double t_max, t_hit *rec)
{
	t_object	*obj;
	int			found;

	found = 0;
	obj = scene->objects;
	while (obj)
	{
		if (hit_object(obj, r, t_max, rec))
		{
			found = 1;
			t_max = rec->t;
		}
		obj = obj->next;
	}
	if (found && vec3_dot(r.direction, rec->normal) > 0.0)
		rec->normal = vec3_negate(rec->normal);
	return (found);
}
