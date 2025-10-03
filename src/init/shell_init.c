/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:19 by pgavel            #+#    #+#             */
/*   Updated: 2025/07/23 11:50:20 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

static void	add_node_to_env(t_shell *shell, t_env *new_node)
{
	t_env	*current;

	if (!shell->env)
		shell->env = new_node;
	else
	{
		current = shell->env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

static void	add_env_var(t_shell *shell, char *env_str)
{
	char	*eq_pos;
	char	*key;
	char	*value;
	t_env	*new_node;

	eq_pos = ft_strchr(env_str, '=');
	if (!eq_pos)
		return ;
	key = ft_substr(env_str, 0, eq_pos - env_str);
	value = ft_strdup(eq_pos + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return ;
	}
	new_node = create_env_node(key, value);
	free(key);
	free(value);
	if (!new_node)
		return ;
	add_node_to_env(shell, new_node);
}

void	init_shell(t_shell *shell, char **envp)
{
	int	i;

	shell->env = NULL;
	shell->exit_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	i = 0;
	while (envp[i])
	{
		add_env_var(shell, envp[i]);
		i++;
	}
	if (!get_env_value(shell, "PATH"))
		set_env_value(shell, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
}

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
