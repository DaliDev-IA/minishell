/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:22:29 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 23:43:48 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_only_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void	process_input(t_shell *shell, char *input)
{
	t_token	*tokens;
	t_cmd	*commands;

	if (*input)
		add_history(input);
	if (is_only_whitespace(input))
		return ;
	tokens = tokenize(input);
	if (!tokens)
	{
		shell->exit_status = 2;
		return ;
	}
	commands = parse_tokens(tokens);
	if (!commands)
	{
		shell->exit_status = 2;
		free_tokens(tokens);
		return ;
	}
	shell->exit_status = execute_commands(shell, commands);
	free_commands(commands);
	free_tokens(tokens);
}

static void	strip_newline(char *str)
{
	int	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';
}

static char	*read_input(void)
{
	char	*input;

	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)
		|| !isatty(STDERR_FILENO))
	{
		input = get_next_line(STDIN_FILENO);
		strip_newline(input);
	}
	else
		input = readline(PROMPT);
	return (input);
}

void	main_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = read_input();
		if (g_signal_received == SIGINT)
		{
			shell->exit_status = 130;
			g_signal_received = 0;
		}
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		process_input(shell, input);
		free(input);
	}
}
