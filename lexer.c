/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:48:45 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/07 19:51:43 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator(char c)
{
	//printf("operator: checking char='%c'\n", c);
	return (c == '|' || c == '<' || c == '>');
}

int	handle_operator(char *input, int *i)
{
	//printf("handle_operator: index=%d, char='%c'\n", *i, input[*i]);
	if (operator(input[*i]))
	{
		if (input[*i] == '|')
		{
			if (operator(input[*i + 1]))
			{
				//printf("Erreur: opérateur invalide après '|'\n");
				return (-1);
			}
			//printf("Opérateur PIPE trouvé\n");
			return (TOKEN_PIPE);
		}
		if (input[*i] == '<')
		{
			if (!operator(input[*i + 1]))
			{
				//printf("Opérateur REDIR_IN trouvé\n");
				return (TOKEN_REDIR_IN);
			}
			else if (input[*i + 1] == '<' && !operator(input[*i + 2]))
			{
				//printf("Opérateur HEREDOC trouvé\n");
				return (TOKEN_HEREDOC);
			}
			//printf("Erreur: opérateur invalide après '<'\n");
			return (-1);
		}
		if (input[*i] == '>')
		{
			if (!operator(input[*i + 1]))
			{
				//printf("Opérateur REDIR_OUT trouvé\n");
				return (TOKEN_REDIR_OUT);
			}
			else if (input[*i + 1] == '>' && !operator(input[*i + 2]))
			{
				//printf("Opérateur APPEND trouvé\n");
				return (TOKEN_APPEND);
			}
			//printf("Erreur: opérateur invalide après '>'\n");
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

	//printf("split_tokens: input='%s'\n", input);
	head = NULL;
	if (!input)
	{
		//printf("Erreur: input NULL\n");
		return (NULL);
	}
	if (check_quotes(input) < 0)
	{
		//printf("Erreur: check_quotes échoué\n");
		return (NULL);
	}
	i = 0;
	while (input[i])
	{
		//printf("split_tokens: index=%d, char='%c'\n", i, input[i]);
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		handle_return = handle_operator(input, &i);
		if (handle_return == -1)
		{
			//printf("Erreur: handle_operator retourné -1\n");
			free_tokens(head);
			return (NULL);
		}
		if (handle_return > 0)
		{
			//printf("Ajout opérateur, type=%d\n", handle_return);
			add_operator_token(&head, handle_return, input, &i);
			continue ;
		}
		//printf("Ajout mot\n");
		add_word_token(&head, input, &i);
	}
	//printf("split_tokens: fin parsing, head=%p\n", head);
	return (head);
}
