/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:01:09 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/03 22:01:10 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
