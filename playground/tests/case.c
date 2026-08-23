#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "case.h"

static FILE	*g_fp;
static int	g_lineno;

int	cs_list(const char *dir, char names[CS_MAXFILES][CS_MAXNAME])
{
	DIR				*d;
	struct dirent	*e;
	int				n;
	int				i;
	int				j;
	char			tmp[CS_MAXNAME];

	d = opendir(dir);
	if (!d)
		return (0);
	n = 0;
	while ((e = readdir(d)) && n < CS_MAXFILES)
	{
		if (strstr(e->d_name, ".case")
			&& strcmp(strstr(e->d_name, ".case"), ".case") == 0)
			snprintf(names[n++], CS_MAXNAME, "%s", e->d_name);
	}
	closedir(d);
	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (strcmp(names[j], names[i]) < 0)
			{
				snprintf(tmp, CS_MAXNAME, "%s", names[i]);
				snprintf(names[i], CS_MAXNAME, "%s", names[j]);
				snprintf(names[j], CS_MAXNAME, "%s", tmp);
			}
			j++;
		}
		i++;
	}
	return (n);
}

int	cs_open(const char *path)
{
	g_fp = fopen(path, "r");
	g_lineno = 0;
	return (g_fp != NULL);
}

void	cs_close(void)
{
	if (g_fp)
		fclose(g_fp);
	g_fp = NULL;
}

int	cs_next(t_case *c)
{
	char	*p;

	while (fgets(c->raw, CS_MAXLINE, g_fp))
	{
		g_lineno++;
		c->lineno = g_lineno;
		p = strchr(c->raw, '\n');
		if (p)
			*p = '\0';
		p = strchr(c->raw, '\r');
		if (p)
			*p = '\0';
		snprintf(c->buf, CS_MAXLINE, "%s", c->raw);
		p = strchr(c->buf, '#');
		if (p)
			*p = '\0';
		c->ntok = 0;
		p = strtok(c->buf, " \t");
		while (p && c->ntok < CS_MAXTOK)
		{
			c->tok[c->ntok++] = p;
			p = strtok(NULL, " \t");
		}
		if (c->ntok > 0)
			return (1);
	}
	return (0);
}

int	cs_vec(const char *s, double *x, double *y, double *z)
{
	return (sscanf(s, "%lf,%lf,%lf", x, y, z) == 3);
}

double	cs_num(const char *s)
{
	return (atof(s));
}

unsigned int	cs_hex(const char *s)
{
	unsigned int	v;

	v = 0;
	if (sscanf(s, "0x%x", &v) == 1 || sscanf(s, "0X%x", &v) == 1)
		return (v);
	return ((unsigned int)strtoul(s, NULL, 10));
}

int	cs_is(const char *tok, const char *s)
{
	return (strcmp(tok, s) == 0);
}

int	cs_arrow(t_case *c)
{
	int	i;

	i = 0;
	while (i < c->ntok)
	{
		if (cs_is(c->tok[i], "=>"))
			return (i + 1);
		i++;
	}
	return (-1);
}
