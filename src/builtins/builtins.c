/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:49:52 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 22:43:41 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_underscore_var(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		i++;
	if (i > 0)
		set_env_value(shell, "_", cmd->args[i - 1]);
}

int	execute_builtin(t_shell *shell, t_cmd *cmd)
{
	int	result;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (setup_redirections(cmd->redirs) == -1)
	{
		restore_redirections(shell);
		return (1);
	}
	result = run_builtin_command(shell, cmd);
	update_underscore_var(shell, cmd);
	restore_redirections(shell);
	return (result);
}
