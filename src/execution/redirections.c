/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:15 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 21:21:27 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_redirections(t_shell *shell)
{
	dup2(shell->stdin_backup, STDIN_FILENO);
	dup2(shell->stdout_backup, STDOUT_FILENO);
}

static int	handle_heredoc_redir(char *file)
{
	int	fd;

	fd = handle_heredoc(file);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	process_redir_type(t_redir *redir)
{
	if (!redir || !redir->file)
		return (-1);
	if (redir->type == TOKEN_REDIR_IN)
		return (handle_input_redir(redir->file));
	else if (redir->type == TOKEN_REDIR_OUT)
		return (handle_output_redir(redir->file, 0));
	else if (redir->type == TOKEN_REDIR_APPEND)
		return (handle_output_redir(redir->file, 1));
	else if (redir->type == TOKEN_REDIR_HEREDOC)
		return (handle_heredoc_redir(redir->file));
	return (0);
}

int	setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (process_redir_type(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}
