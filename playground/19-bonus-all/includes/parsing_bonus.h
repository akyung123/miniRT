/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoykim <seoykim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:59:32 by seoykim           #+#    #+#             */
/*   Updated: 2026/09/17 12:59:32 by seoykim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_BONUS_H
# define PARSING_BONUS_H

# include "miniRT_bonus.h"

/* ---------- String utils (only what's missing from libft) ---------- */

void	free_tokens(char **tokens);
int		count_tokens(char **tokens);
int		has_rt_extension(const char *path);

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
int		parse_cone(char **tokens, t_scene *scene);
void	add_object(t_scene *scene, t_object *obj);

/* ---------- 보너스 표면 옵션 (ck 체크무늬 / bp 범프) ---------- */

t_object	*new_object(t_object_type type);
int		count_ok(char **tokens, int base);
int		parse_modifiers(char **tokens, int i, t_object *obj);

#endif
