/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.kr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:15:00 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/20 16:15:00 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include "miniRT.h"

# define DESTROY_NOTIFY 17

# ifdef __APPLE__
#  define KEY_ESC 53
#  define CLOSE_MASK 0
# else
#  define KEY_ESC 65307
#  define CLOSE_MASK 131072
# endif

int	init_mlx(t_minirt *rt);
int	close_hook(void *param);
int	key_hook(int keycode, void *param);
int	expose_hook(void *param);
void	run_mlx(t_minirt *rt);
void	cleanup_mlx(t_minirt *rt);

#endif
