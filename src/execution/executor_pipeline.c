/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:02 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 15:37:55 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_child_pipes(int prev_fd, int *pipefd, t_cmd *current)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

static void	execute_pipeline_child(t_shell *shell, t_cmd *current,
	int prev_fd, int *pipefd)
{
	setup_child_signals();
	close(shell->stdin_backup);
	close(shell->stdout_backup);
	process_cmd_args(shell, current);
	if (!current->args || !current->args[0])
		exit(127);
	setup_child_pipes(prev_fd, pipefd, current);
	if (setup_redirections(current->redirs) == -1)
		exit(1);
	if (is_builtin(current->args[0]))
		execute_pipeline_builtin(shell, current);
	else
		execute_pipeline_external(shell, current);
}

int	execute_pipeline(t_shell *shell, t_cmd *cmds)
{
	int		pipefd[2];
	pid_t	pid;
	pid_t	last_pid;
	int		prev_fd;
	t_cmd	*current;

	prev_fd = -1;
	current = cmds;
	last_pid = -1;
	while (current)
	{
		if (current->next && pipe(pipefd) == -1)
			return (1);
		pid = fork();
		if (pid == 0)
			execute_pipeline_child(shell, current, prev_fd, pipefd);
		last_pid = pid;
		handle_pipes(&prev_fd, pipefd, current);
		current = current->next;
	}
	return (wait_for_children(last_pid));
}
