/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:23:53 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/20 15:23:55 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "miniRT.h"

/* ---------- String utils (only what's missing from libft) ---------- */

void	free_tokens(char **tokens);
int		count_tokens(char **tokens);

/* ---------- Number / vector / color parsing ---------- */

double	str_to_double(const char *str, int *ok);
int		parse_vec3(const char *str, t_vec3 *out);
int		parse_color(const char *str, t_color *out);
int		normalize_direction(t_vec3 *v);

/* ---------- Line dispatch / per-element parsers ---------- */

int		parse_line(const char *line, t_scene *scene, t_parse_flags *flags);
int		parse_ambient(char **tokens, t_scene *scene, t_parse_flags *flags);
int		parse_camera(char **tokens, t_scene *scene, t_parse_flags *flags);
int		parse_light(char **tokens, t_scene *scene, t_parse_flags *flags);
int		parse_sphere(char **tokens, t_scene *scene);
int		parse_plane(char **tokens, t_scene *scene);
int		parse_cylinder(char **tokens, t_scene *scene);
void	add_object(t_scene *scene, t_object *obj);

#endif
