/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:23:41 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 22:23:42 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_redir_syntax(t_token *current)
{
	if (current->type >= TOKEN_REDIR_IN
		&& current->type <= TOKEN_REDIR_HEREDOC)
	{
		if (!current->next || current->next->type != TOKEN_WORD)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token",
				STDERR_FILENO);
			return (0);
		}
	}
	return (1);
}

static int	validate_tokens_loop(t_token *tokens, t_token **last)
{
	t_token	*current;

	current = tokens;
	*last = NULL;
	while (current)
	{
		if (!check_redir_syntax(current))
			return (0);
		*last = current;
		current = current->next;
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return (0);
	if (!check_pipe_error(tokens->type))
		return (0);
	if (!validate_tokens_loop(tokens, &last))
		return (0);
	if (last && !check_pipe_error(last->type))
		return (0);
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*new_cmd;

	if (!validate_syntax(tokens))
		return (NULL);
	commands = NULL;
	while (tokens)
	{
		new_cmd = create_cmd();
		if (!new_cmd)
		{
			free_commands(commands);
			return (NULL);
		}
		tokens = process_cmd(tokens, new_cmd, &commands);
		if (!tokens)
			break ;
	}
	return (commands);
}
