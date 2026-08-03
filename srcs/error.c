#include <unistd.h>
#include <stdio.h>
#include "miniRT.h"

void	ft_error(const char *msg)
{
	write(2, "Error\n", 6);
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
}

void	ft_sys_error(const char *context)
{
	write(2, "Error\n", 6);
	perror(context);
}
