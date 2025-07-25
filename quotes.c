/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:00:00 by grok             #+#    #+#             */
/*   Updated: 2025/07/17 12:00:00 by grok             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_quotes(char *input)
{
	int i = 0;
	int flag = 0;

	while (input[i])
	{
		if (flag == 0)
		{
			if (input[i] == '\'')
				flag = 1;
			else if (input[i] == '"')
				flag = 2;
		}
		else if (flag == 1 && input[i] == '\'')
			flag = 0;
		else if (flag == 2 && input[i] == '"')
			flag = 0;
		i++;
	}
	if (flag != 0)
	{
		printf("Erreur: quote non fermée !\n");  // Ou error_exit plus tard
		return (-1);
	}
	return (0);
}

int get_token_length_with_quotes(char *input, int *i)
{
	int len = 0;
	int flag = 0;

	while (input[*i + len])
	{
		if (flag == 0 && ft_isspace(input[*i + len]))
			break ;  // Stop sur space hors quote
		if (flag == 0 && operator(input[*i + len]))
			break ;  // Stop sur op hors quote
		if (flag == 0)
		{
			if (input[*i + len] == '\'')
				flag = 1;
			else if (input[*i + len] == '"')
				flag = 2;
		}
		else if (flag == 1 && input[*i + len] == '\'')
			flag = 0;
		else if (flag == 2 && input[*i + len] == '"')
			flag = 0;
		len++;
	}
	if (flag != 0)
		return (-1);
	return (len);
}