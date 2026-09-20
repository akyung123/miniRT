/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:19:46 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/20 15:19:51 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
