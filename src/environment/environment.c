/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:49:56 by pgavel            #+#    #+#             */
/*   Updated: 2025/08/07 19:58:06 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(t_shell *shell, char *key)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env_value(t_shell *shell, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = shell->env;
	shell->env = new_node;
}

void	unset_env_value(t_shell *shell, char *key)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static int	count_env_vars(t_shell *shell)
{
	t_env	*current;
	int		count;

	count = 0;
	current = shell->env;
	while (current)
	{
		if (current->value)
			count++;
		current = current->next;
	}
	return (count);
}

char	**env_to_array(t_shell *shell)
{
	char	**env_array;
	t_env	*current;
	char	*temp;
	int		i;

	env_array = malloc(sizeof(char *) * (count_env_vars(shell) + 1));
	if (!env_array)
		return (NULL);
	current = shell->env;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			temp = ft_strjoin(current->key, "=");
			env_array[i] = ft_strjoin(temp, current->value);
			free(temp);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
