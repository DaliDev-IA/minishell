/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:02 by pgavel            #+#    #+#             */
/*   Updated: 2025/08/07 19:58:15 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

char	*check_path_dirs(char **paths, char *cmd)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_paths(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

void	print_cmd_not_found(char *cmd, int path_exists)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (path_exists)
		ft_putendl_fd(": command not found", STDERR_FILENO);
	else
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
}

void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i])
		free(env_array[i++]);
	free(env_array);
}

int	execute_child_process(t_cmd *cmd, char *cmd_path, char **env_array,
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
