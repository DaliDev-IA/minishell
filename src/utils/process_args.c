/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:03:28 by pgavel            #+#    #+#             */
/*   Updated: 2025/08/07 20:03:35 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	copy_quoted_content(char *str, char *result, int *i, int *j)
{
	char	quote_char;

	quote_char = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote_char)
		result[(*j)++] = str[(*i)++];
	if (str[*i] == quote_char)
		(*i)++;
}

static char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			copy_quoted_content(str, result, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*process_single_arg(t_shell *shell, char *arg)
{
	char	*expanded;
	char	*unquoted;

	if (!arg)
		return (NULL);
	expanded = expand_variables(shell, arg);
	if (!expanded)
		return (ft_strdup(arg));
	unquoted = remove_quotes(expanded);
	free(expanded);
	if (!unquoted)
		return (ft_strdup(arg));
	return (unquoted);
}

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
