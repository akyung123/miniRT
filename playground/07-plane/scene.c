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

void	demo_scene(t_scene *scene)
{
	static t_object	objs[4];

	*scene = (t_scene){0};
	scene->ambient.ratio = 0.2;
	scene->ambient.color = (t_color){1, 1, 1};
	// 왼쪽 위의 광원 → 그림자는 오른쪽 뒤 바닥으로 진다
	scene->light.position = (t_vec3){-4, 5, -1};
	scene->light.brightness = 0.8;
	scene->light.color = (t_color){1, 1, 1};
	// .rt: pl 0,-1,0 0,1,0 220,220,220  → y = -1 높이의 바닥
	set_plane(&objs[0], (t_vec3){0, -1, 0}, (t_vec3){0, 1, 0},
		(t_color){0.85, 0.85, 0.85});
	// 구들은 전부 바닥에 딱 닿게: center.y - 반지름 = -1
	set_sphere(&objs[1], (t_vec3){0.2, 0, -5}, 2.0, (t_color){1, 0.2, 0.2});
	set_sphere(&objs[2], (t_vec3){-2.1, -0.4, -6.2}, 1.2,
		(t_color){0.2, 0.4, 1});
	set_sphere(&objs[3], (t_vec3){2.0, -0.55, -4.0}, 0.9,
		(t_color){0.3, 0.9, 0.4});
	objs[0].next = &objs[1];
	objs[1].next = &objs[2];
	objs[2].next = &objs[3];
	scene->objects = &objs[0];
}
