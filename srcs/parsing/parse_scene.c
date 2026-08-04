#include <stdlib.h>
#include "parsing.h"

int	parse_line(const char *line, t_scene *scene, t_parse_flags *flags)
{
	char	**tokens;
	int		ret;

	tokens = split_char(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), 1);
	if (str_eq(tokens[0], "A"))
		ret = parse_ambient(tokens, scene, flags);
	else if (str_eq(tokens[0], "C"))
		ret = parse_camera(tokens, scene, flags);
	else if (str_eq(tokens[0], "L"))
		ret = parse_light(tokens, scene, flags);
	else if (str_eq(tokens[0], "sp"))
		ret = parse_sphere(tokens, scene);
	else if (str_eq(tokens[0], "pl"))
		ret = parse_plane(tokens, scene);
	else if (str_eq(tokens[0], "cy"))
		ret = parse_cylinder(tokens, scene);
	else
		ret = 0;
	free_tokens(tokens);
	return (ret);
}

static int	process_lines(char **lines, t_scene *scene, t_parse_flags *flags)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (lines[i][0] && !parse_line(lines[i], scene, flags))
		{
			ft_error("invalid or duplicated scene line");
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_scene(const char *path, t_scene *scene)
{
	char			*content;
	char			**lines;
	t_parse_flags	flags;

	scene->objects = NULL;
	flags.has_ambient = 0;
	flags.has_camera = 0;
	flags.has_light = 0;
	content = read_file(path);
	if (!content)
		return (ft_sys_error("open"), 0);
	lines = split_char(content, '\n');
	free(content);
	if (!lines || !process_lines(lines, scene, &flags))
	{
		free_tokens(lines);
		free_scene(scene);
		return (0);
	}
	free_tokens(lines);
	if (!flags.has_ambient || !flags.has_camera || !flags.has_light)
	{
		free_scene(scene);
		ft_error("missing required element (A, C, or L)");
		return (0);
	}
	return (1);
}
