/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUT_BONUS_H
# define OUTPUT_BONUS_H

# include "miniRT_bonus.h"

# ifdef __APPLE__
#  define KEY_ESC 53
#  define CLOSE_MASK 0
# else
#  define KEY_ESC 65307
#  define CLOSE_MASK 131072
# endif

t_color	render_pixel(t_scene *scene, int x, int y);

int		init_mlx(t_minirt *rt);
void	render_all(t_minirt *rt);
int		color_to_int(t_color color);
int		close_hook(void *param);
int		key_hook(int keycode, void *param);
int		expose_hook(void *param);
void	cleanup_mlx(t_minirt *rt);

#endif
