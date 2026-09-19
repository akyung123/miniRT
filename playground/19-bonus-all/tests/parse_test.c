#include <stdio.h>
#include "parsing.h"

int	main(int argc, char **argv)
{
	t_scene		scene;
	t_object	*obj;
	t_light		*light;

	if (argc != 2)
	{
		printf("usage: %s <scene>.rt\n", argv[0]);
		return (1);
	}
	if (!parse_scene(argv[1], &scene))
		return (1);
	printf("ambient: ratio=%.3f color=(%.3f,%.3f,%.3f)\n",
		scene.ambient.ratio, scene.ambient.color.x,
		scene.ambient.color.y, scene.ambient.color.z);
	printf("camera: pos=(%.3f,%.3f,%.3f) dir=(%.3f,%.3f,%.3f) fov=%.3f\n",
		scene.camera.position.x, scene.camera.position.y,
		scene.camera.position.z, scene.camera.orientation.x,
		scene.camera.orientation.y, scene.camera.orientation.z,
		scene.camera.fov);
	light = scene.lights;
	while (light)
	{
		printf("light: pos=(%.3f,%.3f,%.3f) brightness=%.3f "
			"color=(%.3f,%.3f,%.3f)\n",
			light->position.x, light->position.y, light->position.z,
			light->brightness, light->color.x, light->color.y,
			light->color.z);
		light = light->next;
	}
	obj = scene.objects;
	while (obj)
	{
		if (obj->type == OBJ_SPHERE)
			printf("sphere: center=(%.3f,%.3f,%.3f) diameter=%.3f\n",
				obj->data.sphere.center.x, obj->data.sphere.center.y,
				obj->data.sphere.center.z, obj->data.sphere.diameter);
		else if (obj->type == OBJ_PLANE)
			printf("plane: point=(%.3f,%.3f,%.3f) normal=(%.3f,%.3f,%.3f)\n",
				obj->data.plane.point.x, obj->data.plane.point.y,
				obj->data.plane.point.z, obj->data.plane.normal.x,
				obj->data.plane.normal.y, obj->data.plane.normal.z);
		else if (obj->type == OBJ_CYLINDER)
			printf("cylinder: center=(%.3f,%.3f,%.3f) axis=(%.3f,%.3f,%.3f) "
				"diameter=%.3f height=%.3f\n",
				obj->data.cylinder.center.x, obj->data.cylinder.center.y,
				obj->data.cylinder.center.z, obj->data.cylinder.axis.x,
				obj->data.cylinder.axis.y, obj->data.cylinder.axis.z,
				obj->data.cylinder.diameter, obj->data.cylinder.height);
		else if (obj->type == OBJ_CONE)
			printf("cone: center=(%.3f,%.3f,%.3f) axis=(%.3f,%.3f,%.3f) "
				"diameter=%.3f height=%.3f\n",
				obj->data.cone.center.x, obj->data.cone.center.y,
				obj->data.cone.center.z, obj->data.cone.axis.x,
				obj->data.cone.axis.y, obj->data.cone.axis.z,
				obj->data.cone.diameter, obj->data.cone.height);
		if (obj->checker)
			printf("  checker: scale=%.3f color=(%.3f,%.3f,%.3f)\n",
				obj->checker_scale, obj->checker_color.x,
				obj->checker_color.y, obj->checker_color.z);
		if (obj->bump)
			printf("  bump: freq=%.3f strength=%.3f\n",
				obj->bump_freq, obj->bump_strength);
		obj = obj->next;
	}
	free_scene(&scene);
	return (0);
}
