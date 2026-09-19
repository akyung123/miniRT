/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

/* 보너스: 색 교란(체크무늬).
 * 표면 좌표 (u, v) 를 칸 크기로 나눠 내림한 뒤, 두 칸 번호의 합이
 * 홀수면 두 번째 색으로 칠한다. 체스판이 만들어지는 이유가 이것이다.
 */
t_color	apply_checker(t_object *obj, t_hit *rec)
{
	t_uv	uv;
	int		iu;
	int		iv;

	uv = object_uv(obj, rec->point);
	iu = (int)floor(uv.u / obj->checker_scale);
	iv = (int)floor(uv.v / obj->checker_scale);
	if ((iu + iv) & 1)
		return (obj->checker_color);
	return (rec->color);
}
