/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:39:45 by mohchams          #+#    #+#             */
/*   Updated: 2025/07/17 12:00:00 by grok             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tokens(t_token *head)
{
	t_token	*temp;
	t_token	*temp2;

	temp = head;
	while (temp)
	{
		temp2 = temp;
		temp = temp->next;
		free(temp2->value);
		free(temp2);
	}
}

void print_tokens(t_token *head)
{
    t_token *current = head;
    while (current)
    {
        printf("Token type: %d, value: '%s'\n", current->type, current->value);
        current = current->next;
    }
    printf("--- Fin des tokens ---\n");
}

int	main(int ac, char **av, char **envp)
{
	printf("Starting minishell...\n");
	char *input;
	t_token	*l;
	(void)ac;
	(void)av;
	(void)envp;  // Pour futur init_env

	while (1)
	{
		printf("Entering loop...\n");
		input = readline("minishell> ");
		if (!input)
			exit(0);
		if (ft_strncmp("exit", input, 5) == 0)
		{
			printf("exit\n");
			free(input);
			exit(0);
		}
		add_history(input);
		l = split_tokens(input);
		if (l)
		{
    		print_tokens(l);  // Ajoute ça pour debug
    		free_tokens(l);
		}
		free(input);
		if (l)
			free_tokens(l);
	}
	return (0);
}