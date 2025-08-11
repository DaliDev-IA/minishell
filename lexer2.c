/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:34:53 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/08 12:37:39 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*split_tokens(char *input)
{
	int		i;
	t_token	*head;
	int		type;

	head = NULL;
	if (!input)
		return (NULL);
	if (check_quotes(input) < 0)
		return (NULL);
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		type = handle_operator(input, &i);
		if (type == -1)
			return (free_tokens(head), NULL);
		if (type > 0)
			add_operator_token(&head, type, input, &i);
		else
			add_word_token(&head, input, &i);
	}
	return (head);
}
