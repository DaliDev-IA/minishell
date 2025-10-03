/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:00:43 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/03 22:00:45 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipe_error(t_token_typ type)
{
	if (type == TOKEN_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'",
			STDERR_FILENO);
		return (0);
	}
	return (1);
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
