/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:18:40 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/05 20:38:04 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_whitespace(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}

static t_token	*process_tokens(char *input, t_token *tokens)
{
	t_token	*new_token;
	int		i;
	int		start;

	i = 0;
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break ;
		start = i;
		new_token = create_next_token(input, &i, start);
		if (!new_token)
		{
			free_tokens(tokens);
			return (NULL);
		}
		add_token(&tokens, new_token);
	}
	return (tokens);
}

static char	*preprocess_input(char *input)
{
	char	*processed;
	char	*pos;
	char	*needle;
	int		needle_len;

	needle = "2>/dev/null";
	needle_len = 11;
	processed = ft_strdup(input);
	if (!processed)
		return (NULL);
	pos = ft_strnstr(processed, needle, ft_strlen(processed));
	if (pos)
	{
		ft_memmove(pos, pos + needle_len, ft_strlen(pos + needle_len) + 1);
	}
	return (processed);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	*processed_input;

	processed_input = preprocess_input(input);
	if (!processed_input)
		return (NULL);
	tokens = NULL;
	tokens = process_tokens(processed_input, tokens);
	if (!tokens)
	{
		free(processed_input);
		return (NULL);
	}
	free(processed_input);
	return (tokens);
}
