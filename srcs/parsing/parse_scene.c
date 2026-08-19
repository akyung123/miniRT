#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "parsing.h"

int	parse_line(const char *line, t_scene *scene, t_parse_flags *flags)
{
	char	**tokens;
	int		ret;

	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), 1);
	if (!ft_strcmp(tokens[0], "A"))
		ret = parse_ambient(tokens, scene, flags);
	else if (!ft_strcmp(tokens[0], "C"))
		ret = parse_camera(tokens, scene, flags);
	else if (!ft_strcmp(tokens[0], "L"))
		ret = parse_light(tokens, scene, flags);
	else if (!ft_strcmp(tokens[0], "sp"))
		ret = parse_sphere(tokens, scene);
	else if (!ft_strcmp(tokens[0], "pl"))
		ret = parse_plane(tokens, scene);
	else if (!ft_strcmp(tokens[0], "cy"))
		ret = parse_cylinder(tokens, scene);
	else
		ret = 0;
	free_tokens(tokens);
	return (ret);
}

static void	strip_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static int	process_fd(int fd, t_scene *scene, t_parse_flags *flags)
{
	char	*line;
	int		ok;

	ok = 1;
	line = get_next_line(fd);
	while (line)
	{
		strip_newline(line);
		if (ok && line[0] && !parse_line(line, scene, flags))
		{
			ft_error("invalid or duplicated scene line");
			ok = 0;
		}
		free(line);
		line = get_next_line(fd);
	}
	return (ok);
}

int	parse_scene(const char *path, t_scene *scene)
{
	int				fd;
	t_parse_flags	flags;

	scene->objects = NULL;
	flags.has_ambient = 0;
	flags.has_camera = 0;
	flags.has_light = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ft_sys_error("open"), 0);
	if (!process_fd(fd, scene, &flags))
	{
		close(fd);
		free_scene(scene);
		return (0);
	}
	close(fd);
	if (!flags.has_ambient || !flags.has_camera || !flags.has_light)
	{
		free_scene(scene);
		ft_error("missing required element (A, C, or L)");
		return (0);
	}
	return (1);
}
