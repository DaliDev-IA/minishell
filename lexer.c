/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:48:45 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/06 12:59:09 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int handle_operator(char *input, int *i)
{
	if (operator(input[*i]))
	{
		if (input[*i] == '|')
		{
			if (operator(input[*i + 1]))
				return (-1);
			(*i)++;
			return (TOKEN_PIPE);
		}
		if (input[*i] == '<')
		{
			if (!operator(input[*i + 1]))
			{
				(*i)++;
				return (TOKEN_REDIR_IN);
			}
			else if (input[*i + 1] == '<' && !operator(input[*i + 2]))
			{
				(*i) += 2;
				return (TOKEN_HEREDOC);
			}
			return (-1);
		}
		if (input[*i] == '>')
		{
			if (!operator(input[*i + 1]))
			{
				(*i)++;
				return (TOKEN_REDIR_OUT);
			}
			else if (input[*i + 1] == '>' && !operator(input[*i + 2]))
			{
				(*i) += 2;
				return (TOKEN_APPEND);
			}
			return (-1);
		}
	}
	return (0);
}

t_token	*split_tokens(char *input)
{
	int		i;
	t_token	*head;
	int		handle_return;

	head = NULL;
	if (!input)
		return (NULL);
	if (check_quotes(input) < 0)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		handle_return = handle_operator(input, &i);
		if (handle_return == -1)
		{
			free_tokens(head);
			return (NULL);
		}
		if (handle_return > 0)
		{
			add_operator_token(&head, handle_return, input, &i);
			continue ;
		}
		add_word_token(&head, input, &i);
	}
	return (head);
}
