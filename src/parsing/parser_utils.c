/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:12 by pgavel            #+#    #+#             */
/*   Updated: 2025/08/07 20:00:31 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir	*create_redir(t_token_typ type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->fd = -1;
	redir->next = NULL;
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}

void	add_redir(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redirs)
		*redirs = new_redir;
	else
	{
		current = *redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
			count++;
		else if (current->type >= TOKEN_REDIR_IN
			&& current->type <= TOKEN_REDIR_HEREDOC && current->next)
			current = current->next;
		current = current->next;
	}
	return (count);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd_to_list(t_cmd **commands, t_cmd *new_cmd)
{
	t_cmd	*current_cmd;

	if (!*commands)
		*commands = new_cmd;
	else
	{
		current_cmd = *commands;
		while (current_cmd->next)
			current_cmd = current_cmd->next;
		current_cmd->next = new_cmd;
	}
}
