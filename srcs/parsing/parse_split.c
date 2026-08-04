#include <stdlib.h>
#include "parsing.h"

static int	count_words(const char *str, char delim)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != delim && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*str == delim)
			in_word = 0;
		str++;
	}
	return (count);
}

static char	*extract_word(const char *str, int len)
{
	char	*word;
	int		i;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**split_char(const char *str, char delim)
{
	char	**result;
	int		count;
	int		idx;
	int		len;

	count = count_words(str, delim);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	idx = 0;
	while (idx < count)
	{
		while (*str == delim)
			str++;
		len = 0;
		while (str[len] && str[len] != delim)
			len++;
		result[idx] = extract_word(str, len);
		if (!result[idx])
			return (free_tokens(result), NULL);
		str += len;
		idx++;
	}
	result[idx] = NULL;
	return (result);
}
