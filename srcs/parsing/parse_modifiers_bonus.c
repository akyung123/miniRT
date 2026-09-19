/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_modifiers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 10:00:00 by akkim             #+#    #+#             */
/*   Updated: 2026/09/19 10:00:00 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "parsing_bonus.h"

/* 보너스 표면 옵션은 도형 줄 끝에 3토큰 단위로 붙는다.
 *   ck <칸 크기> <두 번째 색>     체크무늬
 *   bp <주기> <세기>              절차적 범프
 * 둘 다 선택이고 순서는 상관없다. 그래서 기본 토큰 수 base 에
 * 0 / 3 / 6 을 더한 값만 허용한다.
 */
t_object	*new_object(t_object_type type)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = type;
	obj->checker = 0;
	obj->checker_scale = 1.0;
	obj->bump = 0;
	obj->bump_freq = 1.0;
	obj->bump_strength = 0.0;
	obj->next = NULL;
	return (obj);
}

int	count_ok(char **tokens, int base)
{
	int	n;

	n = count_tokens(tokens);
	return (n == base || n == base + 3 || n == base + 6);
}

static int	parse_checker(char **tokens, int i, t_object *obj)
{
	int	ok;

	if (obj->checker)
		return (0);
	obj->checker_scale = str_to_double(tokens[i + 1], &ok);
	if (!ok || obj->checker_scale <= 0.0)
		return (0);
	if (!parse_color(tokens[i + 2], &obj->checker_color))
		return (0);
	obj->checker = 1;
	return (1);
}

static int	parse_bump(char **tokens, int i, t_object *obj)
{
	int	ok;

	if (obj->bump)
		return (0);
	obj->bump_freq = str_to_double(tokens[i + 1], &ok);
	if (!ok || obj->bump_freq <= 0.0)
		return (0);
	obj->bump_strength = str_to_double(tokens[i + 2], &ok);
	if (!ok || obj->bump_strength < 0.0)
		return (0);
	obj->bump = 1;
	return (1);
}

int	parse_modifiers(char **tokens, int i, t_object *obj)
{
	int	n;

	n = count_tokens(tokens);
	while (i + 2 < n)
	{
		if (!ft_strcmp(tokens[i], "ck"))
		{
			if (!parse_checker(tokens, i, obj))
				return (0);
		}
		else if (!ft_strcmp(tokens[i], "bp"))
		{
			if (!parse_bump(tokens, i, obj))
				return (0);
		}
		else
			return (0);
		i += 3;
	}
	return (i == n);
}
