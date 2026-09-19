/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_scene_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

/* 씬 전체에서 가장 가까운 교차를 찾는다.
 * 법선을 광선 쪽으로 뒤집은 다음에 범프를 적용해야, 안쪽에서 볼 때도
 * 울퉁불퉁한 방향이 뒤집히지 않는다.
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
	if (found && rec->obj->bump)
		apply_bump(rec->obj, rec);
	return (found);
}
