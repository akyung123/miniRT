/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
파서는 9단계에서 연결한다. 그 전까지는 .rt 대신 코드로 씬을 만든다

값은 파서가 채워주는 것과 같은 단위로 넣는다
  - 구는 반지름이 아니라 '지름'
  - 색은 0~255가 아니라 0.0~1.0 (파서가 /255 해서 넣어줌)

노드는 malloc하지 않고 static 배열에 둔다
  - 프로그램이 끝날 때까지 살아 있으니 free할 필요가 없다
  - 9단계에서 parse_scene이 malloc한 리스트로 바뀐다 (그땐 free_scene)
*/

static void	set_sphere(t_object *obj, t_vec3 center, double diameter,
		t_color color)
{
	obj->type = OBJ_SPHERE;
	obj->data.sphere.center = center;
	obj->data.sphere.diameter = diameter;
	obj->data.sphere.color = color;
	obj->next = NULL;
}

void	demo_scene(t_scene *scene)
{
	static t_object	objs[3];

	*scene = (t_scene){0};
	// 화면 한가운데, 거리 5에 지름 2(반지름 1)짜리 빨간 구
	set_sphere(&objs[0], (t_vec3){0, 0, -5}, 2.0, (t_color){1, 0, 0});
	// 오른쪽 위에서 빨간 구와 겹치는 초록 구. 더 가까우니(z=-3.5) 앞에 그려져야 한다
	set_sphere(&objs[1], (t_vec3){0.7, 0.45, -3.5}, 1.0, (t_color){0, 1, 0});
	// 카메라 뒤(z=+5)의 파란 구. t가 음수라 보이면 안 된다
	set_sphere(&objs[2], (t_vec3){0, 0, 5}, 2.0, (t_color){0, 0, 1});
	objs[0].next = &objs[1];
	objs[1].next = &objs[2];
	scene->objects = &objs[0];
}
