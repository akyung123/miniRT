/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
그림자 = 표면점에서 광원 쪽으로 광선을 '한 번 더' 쏴 보는 것

              ☀ 광원
             ╱
            ╱   ← 그림자 광선: origin = P, direction = normalize(광원 - P)
          ●     ← 사이에 구가 있으면 막힌다 → 그늘
         ╱
   ─────P─────  표면

막혔다고 판단하는 조건:  T_MIN < t < 광원까지의 거리

  t < 0 쪽 (광원 반대편 물체)  → 빛을 막지 않는다
  t > 거리 (광원보다 뒤의 물체) → 빛을 막지 않는다  ← 빠뜨리기 쉬움
  t ≈ 0   (P가 서 있는 표면 자신) → 막으면 안 된다   ← shadow acne

direction을 정규화해야 t가 곧 거리라서 '광원까지의 거리'와 바로 비교할 수 있다
*/
static t_ray	shadow_ray(t_vec3 p, t_vec3 light_pos, double *dist)
{
	t_ray	r;
	t_vec3	to_light;

	to_light = vec3_sub(light_pos, p);
	*dist = vec3_length(to_light);
	r.origin = p;
	r.direction = vec3_normalize(to_light);
	return (r);
}

/*
테스트용 (test_06.c 계약): 구 배열 중 하나라도 P와 광원 사이를 막으면 1

카메라 광선과 다른 점: '가장 가까운' 걸 찾을 필요가 없다
막는 게 하나라도 있으면 그걸로 그늘이 확정이니 바로 return 한다

hit_sphere의 t_min / t_max 를 그대로 활용한다
  t_min = T_MIN  → P 자기 표면을 t≈0에서 다시 맞는 것(acne)을 걸러낸다
  t_max = dist   → 광원보다 뒤에 있는 구를 걸러낸다
4단계에서 범위를 넣어둔 게 여기서 두 가지 문제를 한 번에 막는다
*/
int	shadow_test(t_vec3 p, t_vec3 light_pos, t_sphere_lite *blockers, int n)
{
	t_ray	r;
	double	dist;
	int		i;

	r = shadow_ray(p, light_pos, &dist);
	i = 0;
	while (i < n)
	{
		if (hit_sphere(r, blockers[i].center, blockers[i].radius,
				T_MIN, dist) > 0.0)
			return (1);
		i++;
	}
	return (0);
}

/*
렌더용: 씬의 도형 리스트를 돌면서 같은 판정

7단계: 구만 보던 반복문을 hit_object로 바꿨다
  → 평면(과 8단계 원기둥)도 그림자를 만들고, 구가 바닥에 그림자를 드리운다

hit_scene을 안 쓰고 hit_object를 직접 도는 이유
  hit_scene은 '가장 가까운 것'을 찾느라 끝까지 돈다
  그림자는 하나만 걸려도 그늘이니 첫 교차에서 바로 return 하는 게 빠르다
*/
int	in_shadow(t_scene *scene, t_vec3 p)
{
	t_ray		r;
	t_object	*obj;
	t_hit		tmp;
	double		dist;

	r = shadow_ray(p, scene->light.position, &dist);
	obj = scene->objects;
	while (obj)
	{
		// t_max = dist : 광원보다 뒤의 물체는 무시
		// t_min(T_MIN)은 hit_object 안에서 걸러준다 : 자기 표면(acne) 무시
		if (hit_object(obj, r, dist, &tmp))
			return (1);
		obj = obj->next;
	}
	return (0);
}
