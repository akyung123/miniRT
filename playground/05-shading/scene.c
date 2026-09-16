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
	static t_object	objs[2];

	*scene = (t_scene){0};
	// .rt: A 0.2 255,255,255  → 모든 면에 20%의 흰 빛
	scene->ambient.ratio = 0.2;
	scene->ambient.color = (t_color){1, 1, 1};
	// .rt: L -5,5,0 0.7 255,255,255  → 왼쪽 위 앞에서 비추는 광원
	// 이 위치를 옮기면 구의 밝은 부분이 따라 움직여야 한다
	scene->light.position = (t_vec3){-5, 5, 0};
	scene->light.brightness = 0.7;
	scene->light.color = (t_color){1, 1, 1};
	set_sphere(&objs[0], (t_vec3){0.6, 0, -5}, 2.0, (t_color){1, 0.2, 0.2});
	set_sphere(&objs[1], (t_vec3){-1.6, -0.3, -6}, 1.4,
		(t_color){0.2, 0.4, 1});
	objs[0].next = &objs[1];
	scene->objects = &objs[0];
}
