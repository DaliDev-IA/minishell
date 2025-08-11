/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:20:25 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/07 11:20:25 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_flag(int *flag, char c)
{
	if (*flag == 0)
	{
		if (c == '\'')
			*flag = 1;
		else if (c == '"')
			*flag = 2;
		return (0);
	}
	if (*flag == 1 && c == '\'')
		*flag = 0;
	if (*flag == 2 && c == '"')
		*flag = 0;
	return (0);
}

int	check_quotes(char *input)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (input[i])
	{
		update_flag(&flag, input[i]);
		i++;
	}
	if (flag != 0)
		return (printf("Erreur: quote non fermée !\n"), -1);
	return (0);
}

static int	should_stop(int flag, char c)
{
	if (flag == 0 && ft_isspace(c))
		return (1);
	if (flag == 0 && operator(c))
		return (1);
	return (0);
}

int	get_token_length_with_quotes(char *input, int *i)
{
	int	len;
	int	flag;

	len = 0;
	flag = 0;
	while (input[*i + len])
	{
		if (should_stop(flag, input[*i + len]))
			break ;
		update_flag(&flag, input[*i + len]);
		len++;
	}
	if (flag != 0)
		return (-1);
	return (len);
}
