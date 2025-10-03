/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:59:13 by pgavel            #+#    #+#             */
/*   Updated: 2025/04/19 11:13:27 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *str, char c)
{
	int	i;
	int	trigger;

	i = 0;
	trigger = 0;
	while (*str)
	{
		if (*str != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (i);
}

static char	*word_dup(const char *str, int start, int finish)
{
	char	*word;
	int		i;

	word = malloc((finish - start + 1) * sizeof(char));
	if (!word)
	{
		return (NULL);
	}
	i = 0;
	while (start < finish)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

static char	**free_split(char **split, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

static int	fill_split(const char *s, char c, char **split)
{
	size_t		i;
	int			j;
	int			index;

	i = 0;
	j = 0;
	index = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			split[j] = word_dup(s, index, i);
			if (!split[j])
			{
				return (free_split(split, j) != NULL);
			}
			j++;
			index = -1;
		}
		i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**split;

	if (!s)
	{
		return (NULL);
	}
	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!split)
	{
		return (NULL);
	}
	if (!fill_split(s, c, split))
	{
		return (NULL);
	}
	split[count_words(s, c)] = NULL;
	return (split);
}
