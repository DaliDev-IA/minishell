/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:16 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 15:41:26 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_heredoc(int fd[2])
{
	char	*symbolic_link;
	char	*fd_char;

	fd[0] = open("/tmp", O_TMPFILE | O_RDWR, 0600);
	if (fd[0] == -1)
		return (-1);
	fd_char = ft_itoa(fd[0]);
	if (!fd_char)
		return (close(fd[0]), -1);
	symbolic_link = ft_strjoin("/proc/self/fd/", fd_char);
	free(fd_char);
	if (!symbolic_link)
		return (close(fd[0]), -1);
	fd[1] = open(symbolic_link, O_RDWR);
	free(symbolic_link);
	if (fd[1] == -1)
		return (close(fd[0]), -1);
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;
	int		len;

	if (!delimiter || open_heredoc(pipefd) == -1)
		return (-1);
	len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\0')
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

int	handle_input_redir(char *file)
{
	int	fd;

	if (!file)
		return (-1);
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redir(char *file, int append)
{
	int	fd;
	int	flags;

	if (!file)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
