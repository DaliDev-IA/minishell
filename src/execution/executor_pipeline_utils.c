/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:02 by pgavel            #+#    #+#             */
/*   Updated: 2025/08/07 19:59:49 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_pipeline_builtin(t_shell *shell, t_cmd *current)
{
	exit(execute_builtin(shell, current));
}

void	execute_pipeline_external(t_shell *shell, t_cmd *current)
{
	char	*cmd_path;
	char	**env_array;
	char	*path_env;

	cmd_path = find_command_path(current->args[0], shell);
	env_array = env_to_array(shell);
	if (!cmd_path)
	{
		path_env = get_env_value(shell, "PATH");
		print_cmd_not_found(current->args[0], path_env != NULL);
		exit(127);
	}
	execve(cmd_path, current->args, env_array);
	exit(127);
}

void	handle_pipes(int *prev_fd, int *pipefd, t_cmd *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

static int	process_child_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		return (128 + sig);
	}
	return (1);
}

int	wait_for_children(pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	wpid;

	last_status = 0;
	while (1)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == -1)
			break ;
		if (wpid == last_pid)
			last_status = process_child_status(status);
	}
	return (last_status);
}
