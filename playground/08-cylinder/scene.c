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

static void	set_plane(t_object *obj, t_vec3 point, t_vec3 normal,
		t_color color)
{
	obj->type = OBJ_PLANE;
	obj->data.plane.point = point;
	// 파서는 방향 벡터를 정규화해서 넣어준다. 손으로 넣을 때도 똑같이
	obj->data.plane.normal = vec3_normalize(normal);
	obj->data.plane.color = color;
	obj->next = NULL;
}

// 원기둥은 필드가 많아서 t_cylinder를 통째로 받는다
// 필드 순서: center, axis, diameter, height, color
static void	set_cylinder(t_object *obj, t_cylinder cy)
{
	obj->type = OBJ_CYLINDER;
	// 축은 반드시 단위벡터. 투영 공식 (v·a)a 는 a 길이가 1일 때만 맞다
	// 파서는 정규화해서 주지만, 손으로 넣을 땐 직접 해야 한다 (1순위 실수)
	cy.axis = vec3_normalize(cy.axis);
	obj->data.cylinder = cy;
	obj->next = NULL;
}

void	demo_scene(t_scene *scene)
{
	static t_object	objs[5];

	*scene = (t_scene){0};
	scene->ambient.ratio = 0.2;
	scene->ambient.color = (t_color){1, 1, 1};
	scene->light.position = (t_vec3){-4, 5, -1};
	scene->light.brightness = 0.8;
	scene->light.color = (t_color){1, 1, 1};
	set_plane(&objs[0], (t_vec3){0, -1, 0}, (t_vec3){0, 1, 0},
		(t_color){0.85, 0.85, 0.85});
	set_sphere(&objs[1], (t_vec3){-0.4, 0, -5.5}, 2.0,
		(t_color){1, 0.2, 0.2});
	// .rt: cy -2.2,-0.3,-4.4 0,1,0 1.2 1.4 51,191,204  세워진 원기둥
	// center는 '중심'이라 y = 바닥(-1) + 높이/2 = -0.3
	set_cylinder(&objs[2], (t_cylinder){{-2.2, -0.3, -4.4}, {0, 1, 0},
		1.2, 1.4, {0.2, 0.75, 0.8}});
	// 바닥에 눕힌 원기둥: 축이 수평 → 카메라 쪽 뚜껑이 보인다
	// 반지름 0.4 라 center.y = -1 + 0.4
	set_cylinder(&objs[3], (t_cylinder){{2.3, -0.6, -4.0}, {-1, 0, 0.8},
		0.8, 1.6, {1, 0.8, 0.25}});
	// 비스듬히 기울어진 가는 원기둥 (임의 축 확인용)
	set_cylinder(&objs[4], (t_cylinder){{0.9, 0.9, -7}, {1, 1, 0.5},
		0.5, 2.2, {0.6, 0.4, 1}});
	objs[0].next = &objs[1];
	objs[1].next = &objs[2];
	objs[2].next = &objs[3];
	objs[3].next = &objs[4];
	scene->objects = &objs[0];
}
