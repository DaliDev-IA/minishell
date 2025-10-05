/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:22:42 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 23:31:20 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	wait_and_cleanup(pid_t pid, char *cmd_path, char **env_array)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free_env_array(env_array);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		return (128 + sig);
	}
	return (1);
}

static int	execute_child_process(t_cmd *cmd, char *cmd_path, char **env_array,
	t_shell *shell)
{
	setup_child_signals();
	close(shell->stdin_backup);
	close(shell->stdout_backup);
	if (setup_redirections(cmd->redirs) == -1)
		exit(1);
	execve(cmd_path, cmd->args, env_array);
	exit(127);
}

static int	check_and_find_path(t_cmd *cmd, char **cmd_path, t_shell *shell)
{
	int		err;
	char	*path_env;

	if (ft_strchr(cmd->args[0], '/'))
	{
		err = check_file_error(cmd->args[0]);
		if (err != 0)
			return (err);
	}
	*cmd_path = find_command_path(cmd->args[0], shell);
	if (!*cmd_path)
	{
		path_env = get_env_value(shell, "PATH");
		print_cmd_not_found(cmd->args[0], path_env != NULL);
		return (127);
	}
	return (0);
}

int	execute_external_cmd(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;
	int		err;

	if (!cmd->args || !cmd->args[0])
		return (0);
	err = check_and_find_path(cmd, &cmd_path, shell);
	if (err != 0)
		return (err);
	set_env_value(shell, "_", cmd_path);
	env_array = env_to_array(shell);
	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, cmd_path, env_array, shell);
	else if (pid > 0)
		return (wait_and_cleanup(pid, cmd_path, env_array));
	free(cmd_path);
	return (1);
}
