/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:02 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 17:45:33 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_single_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	process_cmd_args(shell, cmd);
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(shell, cmd));
	return (execute_external_cmd(shell, cmd));
}

int	execute_commands(t_shell *shell, t_cmd *cmds)
{
	int	exit_status;

	if (!cmds)
		return (0);
	g_signal_received = -1;
	if (!cmds->next)
		exit_status = execute_single_cmd(shell, cmds);
	else
		exit_status = execute_pipeline(shell, cmds);
	g_signal_received = 0;
	return (exit_status);
}
