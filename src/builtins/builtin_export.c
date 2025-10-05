/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:49:42 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 22:38:04 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	export_variable(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	char	*existing_value;

	parse_export_arg(arg, &key, &value);
	if (!value)
	{
		existing_value = get_env_value(shell, key);
		if (existing_value)
			value = ft_strdup(existing_value);
	}
	if (!is_valid_identifier(key))
	{
		handle_invalid_identifier(arg, key, value);
		return (1);
	}
	set_env_value(shell, key, value);
	free(key);
	free(value);
	return (0);
}

static void	print_export_var(t_env *env)
{
	ft_putstr_fd("export ", STDOUT_FILENO);
	ft_putstr_fd(env->key, STDOUT_FILENO);
	if (env->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
}

static void	print_all_exports(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, "_") != 0)
			print_export_var(current);
		current = current->next;
	}
}

int	builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_all_exports(shell);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (export_variable(shell, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
