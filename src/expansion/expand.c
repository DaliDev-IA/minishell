/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:06 by pgavel            #+#    #+#             */
/*   Updated: 2025/09/18 13:51:21 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_var_start(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

static char	*append_char(char *dst, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (append_str(dst, buf));
}

static char	*expand_at(t_shell *shell, char *str, int *i, char *acc)
{
	char	*name;
	char	*value;

	if (!str[*i + 1] || !is_var_start(str[*i + 1]))
		return (append_char(acc, str[(*i)++]));
	(*i)++;
	name = get_var_name(str, i);
	if (!name)
	{
		free(acc);
		return (NULL);
	}
	value = get_var_value(shell, name);
	free(name);
	if (!value)
	{
		free(acc);
		return (NULL);
	}
	acc = append_str(acc, value);
	free(value);
	return (acc);
}

static char	*process_quote(char *out, char c, int *in_sq, int *in_dq)
{
	if (c == '\'' && !*in_dq)
		*in_sq = !*in_sq;
	else if (c == '"' && !*in_sq)
		*in_dq = !*in_dq;
	return (append_char(out, c));
}

char	*expand_variables(t_shell *shell, char *str)
{
	int		i;
	int		in_squote;
	int		in_dquote;
	char	*out;

	if (!str)
		return (NULL);
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	out = ft_strdup("");
	if (!out)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' && !in_dquote) || (str[i] == '"' && !in_squote))
			out = process_quote(out, str[i++], &in_squote, &in_dquote);
		else if (str[i] == '$' && !in_squote)
			out = expand_at(shell, str, &i, out);
		else
			out = append_char(out, str[i++]);
		if (!out)
			return (NULL);
	}
	return (out);
}
