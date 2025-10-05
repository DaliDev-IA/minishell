/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:18:40 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/05 21:05:43 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*handle_redir_token(t_token *tokens, t_cmd *cmd)
{
	t_token_typ	redir_type;
	t_redir		*new_redir;

	redir_type = tokens->type;
	tokens = tokens->next;
	if (!tokens || tokens->type != TOKEN_WORD)
		return (NULL);
	new_redir = create_redir(redir_type, tokens->value);
	if (!new_redir)
		return (NULL);
	add_redir(&cmd->redirs, new_redir);
	return (tokens);
}

static t_token	*handle_word_token(t_token *tokens, t_cmd *cmd, int *i)
{
	cmd->args[*i] = ft_strdup(tokens->value);
	(*i)++;
	return (tokens);
}

static t_token	*process_token(t_token *tokens, t_cmd *cmd, int *i)
{
	if (tokens->type == TOKEN_WORD)
		tokens = handle_word_token(tokens, cmd, i);
	else if (tokens->type >= TOKEN_REDIR_IN
		&& tokens->type <= TOKEN_REDIR_HEREDOC)
	{
		tokens = handle_redir_token(tokens, cmd);
		if (!tokens)
			return (NULL);
	}
	return (tokens);
}

static t_token	*parse_single_cmd(t_token *tokens, t_cmd *cmd)
{
	int	i;

	cmd->args = init_args_array(count_args(tokens));
	if (!cmd->args)
		return (NULL);
	i = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		tokens = process_token(tokens, cmd, &i);
		if (!tokens)
			return (NULL);
		tokens = tokens->next;
	}
	return (tokens);
}

t_token	*process_cmd(t_token *tokens, t_cmd *new_cmd, t_cmd **commands)
{
	tokens = parse_single_cmd(tokens, new_cmd);
	if (!tokens && (!new_cmd->args || !new_cmd->args[0]))
	{
		free_commands(new_cmd);
		return (NULL);
	}
	add_cmd_to_list(commands, new_cmd);
	if (tokens && tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	return (tokens);
}
