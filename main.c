/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:39:45 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/07 18:50:18 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tokens(t_token *head)
{
    t_token *temp;
    t_token *temp2;

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
        printf("Token type: %d, value: %s\n", current->type, current->value);
        current = current->next;
    }
    //printf("--- Fin des tokens ---\n");
}

int main(int ac, char **av, char **envp)
{
    char *input;
    t_token *token;
    (void)ac;
    (void)av;
    (void)envp;

    printf("Starting minishell...\n");
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            exit(0);
        }
        if (ft_strncmp("exit", input, 5) == 0)
        {
            printf("exit\n");
            free(input);
            exit(0);
        }
        if (strncmp("clear", input, 6) == 0)
        {
            printf("history clear\n");
            rl_clear_history();
            continue;
        }
        if (input[0] != '\0')
            add_history(input);
        token = split_tokens(input);
        if (token)
        {
            print_tokens(token);
            free_tokens(token);
        }
       else
    	  printf("Erreur: parsing des tokens échoué (ex. quote non fermée ou opérateur invalide)\n");
        free(input);
    }
    rl_clear_history();
    return (0);
}
