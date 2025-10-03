/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:22:59 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 22:23:00 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	execute_builtin_part1(t_shell *shell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(shell, cmd->args));
	return (-1);
}

static int	execute_builtin_part2(t_shell *shell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(shell));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], ":") == 0)
		return (0);
	else if (ft_strcmp(cmd->args[0], "!") == 0)
		return (1);
	return (1);
}

int	run_builtin_command(t_shell *shell, t_cmd *cmd)
{
	int	result;

	result = execute_builtin_part1(shell, cmd);
	if (result != -1)
		return (result);
	return (execute_builtin_part2(shell, cmd));
}
