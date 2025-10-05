/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:09 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 14:22:16 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	g_signal_received = 0;

void	cleanup_shell(t_shell *shell)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	close(shell->stdin_backup);
	close(shell->stdout_backup);
}

static int	handle_args(int argc, char **argv)
{
	if (argc > 1)
	{
		if (ft_strcmp(argv[1], "--version") == 0)
		{
			ft_putstr_fd("minishell version 1.0\n", STDOUT_FILENO);
			return (0);
		}
		else
		{
			ft_putstr_fd("minishell: invalid option\n", STDERR_FILENO);
			return (1);
		}
	}
	return (-1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		arg_result;

	arg_result = handle_args(argc, argv);
	if (arg_result != -1)
		return (arg_result);
	init_shell(&shell, envp);
	setup_signals();
	main_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
