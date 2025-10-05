/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:58:40 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/05 20:35:32 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_quotes(char *input, int start, char quote_char)
{
	int	i;

	i = start + 1;
	while (input[i] && input[i] != quote_char)
		i++;
	if (input[i] != quote_char)
		return (-1);
	i++;
	return (i);
}

int	get_word_end(char *input, int start)
{
	int	i;
	int	quote_end;

	i = start;
	while (input[i] && input[i] != ' ' && input[i] != '\t'
		&& input[i] != '|' && input[i] != '<' && input[i] != '>'
		&& input[i] != '&' && input[i] != ';')
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote_end = handle_quotes(input, i, input[i]);
			if (quote_end == -1)
				return (-1);
			i = quote_end;
		}
		else
			i++;
	}
	return (i);
}

t_token_typ	get_redirect_type(char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			*i += 2;
			return (TOKEN_REDIR_HEREDOC);
		}
		*i += 1;
		return (TOKEN_REDIR_IN);
	}
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			*i += 2;
			return (TOKEN_REDIR_APPEND);
		}
		*i += 1;
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
}
