/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_processor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:03:28 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 23:16:58 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_redir_files(t_shell *shell, t_redir *redirs)
{
	t_redir	*current;
	char	*processed;

	current = redirs;
	while (current)
	{
		if (current->file)
		{
			processed = process_single_arg(shell, current->file);
			if (processed)
			{
				free(current->file);
				current->file = processed;
			}
		}
		current = current->next;
	}
}

void	rebuild_args(t_cmd *cmd, char ***all_splits, int count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *)
			* (count_total_args(all_splits, count) + 1));
	build_new_args(new_args, all_splits, count);
	free_splits(all_splits, count);
	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	cmd->args = new_args;
}

char	***prepare_splits(t_shell *shell, t_cmd *cmd, int *count)
{
	int		i;
	char	*processed;
	char	***all_splits;

	i = 0;
	while (cmd->args[i])
		i++;
	*count = i;
	all_splits = malloc(sizeof(char **) * *count);
	i = 0;
	while (i < *count)
	{
		processed = process_single_arg(shell, cmd->args[i]);
		all_splits[i] = split_unquoted_arg(processed, cmd->args[i]);
		free(processed);
		i++;
	}
	return (all_splits);
}

void	process_cmd_args(t_shell *shell, t_cmd *cmd)
{
	int		count;
	char	***all_splits;

	if (!cmd || !cmd->args)
		return ;
	all_splits = prepare_splits(shell, cmd, &count);
	rebuild_args(cmd, all_splits, count);
	cmd->args = filter_empty_args(cmd->args);
	process_redir_files(shell, cmd->redirs);
}
