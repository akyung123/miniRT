#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "parsing.h"

static char	*join_free(char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;
	size_t	i;

	len1 = 0;
	while (s1 && s1[len1])
		len1++;
	len2 = 0;
	while (s2[len2])
		len2++;
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (free(s1), NULL);
	i = 0;
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}
	len2 = 0;
	while (s2[len2])
		res[i++] = s2[len2++];
	res[i] = '\0';
	free(s1);
	return (res);
}

char	*read_file(const char *path)
{
	int		fd;
	char	buf[4096];
	char	*content;
	ssize_t	n;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	content = malloc(1);
	if (!content)
		return (close(fd), NULL);
	content[0] = '\0';
	n = read(fd, buf, sizeof(buf) - 1);
	while (n > 0)
	{
		buf[n] = '\0';
		content = join_free(content, buf);
		if (!content)
			return (close(fd), NULL);
		n = read(fd, buf, sizeof(buf) - 1);
	}
	close(fd);
	if (n < 0)
		return (free(content), NULL);
	return (content);
}
