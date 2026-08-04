#ifndef PARSING_H
# define PARSING_H

# include "miniRT.h"

/* ---------- 파일 읽기 / 문자열 유틸 (libft 없이 자체 구현) ---------- */

char	*read_file(const char *path);
char	**split_char(const char *str, char delim);
void	free_tokens(char **tokens);
int		count_tokens(char **tokens);
int		str_eq(const char *a, const char *b);

/* ---------- 숫자 / 벡터 / 색상 파싱 ---------- */

double	str_to_double(const char *str, int *ok);
int		parse_vec3(const char *str, t_vec3 *out);
int		parse_color(const char *str, t_color *out);
int		valid_direction(t_vec3 *v);

/* ---------- 라인 디스패치 / 요소별 파서 ---------- */

int		parse_line(const char *line, t_scene *scene, t_parse_flags *flags);
int		parse_ambient(char **tokens, t_scene *scene, t_parse_flags *flags);
int		parse_camera(char **tokens, t_scene *scene, t_parse_flags *flags);
int		parse_light(char **tokens, t_scene *scene, t_parse_flags *flags);
int		parse_sphere(char **tokens, t_scene *scene);
int		parse_plane(char **tokens, t_scene *scene);
int		parse_cylinder(char **tokens, t_scene *scene);
void	add_object(t_scene *scene, t_object *obj);

#endif
