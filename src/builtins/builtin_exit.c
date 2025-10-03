/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:49:38 by pgavel            #+#    #+#             */
/*   Updated: 2025/07/23 11:49:39 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_overflow(char *str, int sign)
{
	int	len;

	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	if (sign == 1 && ft_strcmp(str, "9223372036854775807") > 0)
		return (1);
	if (sign == -1 && ft_strcmp(str, "9223372036854775808") > 0)
		return (1);
	return (0);
}

static int	is_numeric(char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (check_overflow(&str[(str[0] == '+' || str[0] == '-')], sign))
		return (0);
	return (1);
}

int	builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
	{
		cleanup_shell(shell);
		exit(shell->exit_status);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		cleanup_shell(shell);
		exit(2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	cleanup_shell(shell);
	exit(exit_code % 256);
}
