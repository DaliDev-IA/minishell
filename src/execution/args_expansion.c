/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:55:40 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 17:58:38 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

char	**split_unquoted_arg(char *arg, char *original)
{
	char	**split;

	if (has_quotes(original))
	{
		split = malloc(sizeof(char *) * 2);
		if (!split)
			return (NULL);
		split[0] = ft_strdup(arg);
		split[1] = NULL;
		return (split);
	}
	return (ft_split(arg, ' '));
}

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
