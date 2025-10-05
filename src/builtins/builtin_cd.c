/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:49:23 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 22:36:19 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_env_vars(t_shell *shell, char *old_pwd)
{
	char	cwd[MAX_PATH];

	set_env_value(shell, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(shell, "PWD", cwd);
}

static int	handle_cd_error(char *target_dir, char *old_pwd)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(target_dir, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	free(target_dir);
	free(old_pwd);
	return (1);
}

static char	*get_target_dir(t_shell *shell, char **args)
{
	char	*target;

	if (!args[1])
	{
		target = get_env_value(shell, "HOME");
		if (!target)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		return (ft_strdup(target));
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value(shell, "OLDPWD");
		if (!target)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			return (NULL);
		}
		ft_putendl_fd(target, STDOUT_FILENO);
		return (ft_strdup(target));
	}
	return (ft_strdup(args[1]));
}

int	builtin_cd(t_shell *shell, char **args)
{
	char	*target_dir;
	char	*old_pwd;
	char	cwd[MAX_PATH];

	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	target_dir = get_target_dir(shell, args);
	if (!target_dir)
		return (1);
	if (getcwd(cwd, sizeof(cwd)))
		old_pwd = ft_strdup(cwd);
	else
		old_pwd = ft_strdup("");
	if (chdir(target_dir) == -1)
		return (handle_cd_error(target_dir, old_pwd));
	update_env_vars(shell, old_pwd);
	free(target_dir);
	free(old_pwd);
	return (0);
}
