/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/16 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// P(t) = origin + t * direction
// 광선 위를 t만큼 나아간 '지점'을 돌려준다.
// direction이 단위벡터면 t는 곧 출발점으로부터의 거리다.

// ray가 origin과 direction(단위벡터)로 이루어짐
// add는 +
// scale은 X
t_vec3	ray_at(t_ray r, double t)
{
	return (vec3_add(r.origin, vec3_scale(r.direction, t)));
}

// 3단계의 '원점에 고정된 카메라' camera_ray(x, y)는 9단계에서 camera.c로 옮겨갔다
// (임의 위치/방향/fov 를 받는 camera_ray(cam, x, y))
