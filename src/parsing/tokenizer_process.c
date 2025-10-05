/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:48:49 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/05 20:41:33 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*process_word_token(char *input, int *i, int start)
{
	t_token	*new_token;
	int		end;

	end = get_word_end(input, start);
	if (end == -1)
	{
		ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
		ft_putendl_fd("unclosed quotes", STDERR_FILENO);
		return (NULL);
	}
	*i = end;
	new_token = create_token(TOKEN_WORD, ft_substr(input, start, *i - start));
	return (new_token);
}

static t_token	*handle_invalid_token(char c)
{
	ft_putstr_fd("minishell: syntax error near ", STDERR_FILENO);
	ft_putstr_fd("unexpected token `", STDERR_FILENO);
	write(STDERR_FILENO, &c, 1);
	ft_putendl_fd("'", STDERR_FILENO);
	return (NULL);
}

static t_token	*process_redir_token(char *input, int *i, int start)
{
	t_token_typ	type;
	t_token		*new_token;

	type = get_redirect_type(input, i);
	new_token = create_token(type, ft_substr(input, start, *i - start));
	return (new_token);
}

static t_token	*process_pipe_token(char *input, int *i)
{
	t_token	*new_token;

	(void)input;
	new_token = create_token(TOKEN_PIPE, ft_strdup("|"));
	if (!new_token || !new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	(*i)++;
	return (new_token);
}

t_token	*create_next_token(char *input, int *i, int start)
{
	if (input[*i] == '|')
		return (process_pipe_token(input, i));
	else if (input[*i] == '<' || input[*i] == '>')
		return (process_redir_token(input, i, start));
	else if (input[*i] == '&' || input[*i] == ';')
		return (handle_invalid_token(input[*i]));
	else
		return (process_word_token(input, i, start));
}
