/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:48:45 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/08 12:36:07 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	handle_pipe(char *input, int *i)
{
	if (operator(input[*i + 1]))
		return (-1);
	return (TOKEN_PIPE);
}

static int	handle_redirect_in(char *input, int *i)
{
	if (!operator(input[*i + 1]))
		return (TOKEN_REDIR_IN);
	if (input[*i + 1] == '<' && !operator(input[*i + 2]))
		return (TOKEN_HEREDOC);
	return (-1);
}

static int	handle_redirect_out(char *input, int *i)
{
	if (!operator(input[*i + 1]))
		return (TOKEN_REDIR_OUT);
	if (input[*i + 1] == '>' && !operator(input[*i + 2]))
		return (TOKEN_APPEND);
	return (-1);
}

int	handle_operator(char *input, int *i)
{
	if (input[*i] == '|')
		return (handle_pipe(input, i));
	if (input[*i] == '<')
		return (handle_redirect_in(input, i));
	if (input[*i] == '>')
		return (handle_redirect_out(input, i));
	return (0);
}
