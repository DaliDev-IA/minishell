/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:06 by pgavel            #+#    #+#             */
/*   Updated: 2025/08/07 20:03:14 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup(""));
	var_name = ft_substr(str, start, *i - start);
	return (var_name);
}

char	*get_var_value(t_shell *shell, char *var_name)
{
	char	*value;
	char	*exit_status_str;

	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_status_str = ft_itoa(shell->exit_status);
		if (!exit_status_str)
			return (ft_strdup(""));
		return (exit_status_str);
	}
	value = get_env_value(shell, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static int	process_var_length(t_shell *shell, char *str, int *i)
{
	char	*var_name;
	char	*var_value;
	int		len;

	(*i)++;
	var_name = get_var_name(str, i);
	var_value = get_var_value(shell, var_name);
	len = ft_strlen(var_value);
	free(var_name);
	free(var_value);
	return (len);
}

int	count_expanded_length(t_shell *shell, char *str)
{
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
			len += process_var_length(shell, str, &i);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*append_str(char *dst, const char *src)
{
	char	*joined;

	if (!dst && !src)
		return (NULL);
	if (!dst)
		return (ft_strdup(src));
	if (!src)
		return (dst);
	joined = ft_strjoin(dst, src);
	free(dst);
	return (joined);
}
