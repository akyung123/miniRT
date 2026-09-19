/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUT_H
# define OUTPUT_H

# include "miniRT.h"

/* mlx_hook 의 DestroyNotify(17) 는 리눅스에서 마스크가 0이면 안 온다.
 * (1L << 17) = StructureNotifyMask. macOS mlx 는 마스크를 안 본다.
 */
# define DESTROY_NOTIFY 17

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
