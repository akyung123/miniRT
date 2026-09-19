/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

/* 보너스: 절차적 범프 매핑.
 * 실제 형태는 그대로 두고 법선만 흔들어서 울퉁불퉁해 보이게 한다.
 * 높이함수 h(u, v) = sin(f*u) * sin(f*v) 의 기울기만큼 법선을
 * 접평면 방향으로 밀어 준다. 형태가 아니라 음영만 바뀌므로
 * 실루엣은 매끈한 채로 표면만 오돌토돌해진다.
 */
void	apply_bump(t_object *obj, t_hit *rec)
{
	t_uv	uv;
	t_tb	tb;
	t_vec3	shift;
	double	f;

	f = obj->bump_freq;
	uv = object_uv(obj, rec->point);
	tb = object_tb(obj, rec);
	if (vec3_dot(rec->normal, vec3_cross(tb.t, tb.b)) < 0.0)
	{
		tb.t = vec3_negate(tb.t);
		tb.b = vec3_negate(tb.b);
	}
	shift = vec3_add(vec3_scale(tb.t, cos(f * uv.u) * sin(f * uv.v)),
			vec3_scale(tb.b, sin(f * uv.u) * cos(f * uv.v)));
	shift = vec3_sub(rec->normal, vec3_scale(shift, obj->bump_strength));
	if (vec3_length(shift) < 1e-8)
		return ;
	rec->normal = vec3_normalize(shift);
}
