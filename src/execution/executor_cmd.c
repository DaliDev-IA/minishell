/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:22:50 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 22:22:51 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
