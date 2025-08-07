/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:34:06 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/07 19:40:41 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_word_token(t_token **head, char *input, int *i)
{
	t_token	*token;
	int		len;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return ;
	len = get_token_length_with_quotes(input, i);
	if (len < 0)
	{
		free(token);
		return ;
	}
	token->type = TOKEN_WORD;
	token->value = ft_substr(input + *i, 0, len);
	if (!token->value)
	{
		free(token);
		return ;
	}
	*i += len;
	token->next = NULL;
	if (!*head)
		*head = token;
	else
		add_token_back(head, token);
}

void	add_token_back(t_token **head, t_token *new)
{
	t_token	*current;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
	new->next = NULL;
}

void	add_operator_token(t_token **head,
		int handle_return, char *input, int *i)
{
	t_token	*token;
	int		len;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return ;
	if (handle_return >= 1 && handle_return <= 3)
		len = 1;
	else
		len = 2;
	token->type = handle_return;
	token->value = ft_substr(input + *i, 0, len);
	//printf("value token : %s\n", token->value);
	//fflush(stdout);
	if (!token->value)
	{
		free(token);
		return ;
	}
	*i += len;
	token->next = NULL;
	if (!*head)
		*head = token;
	else
		add_token_back(head, token);
}
