/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:48:45 by mohchams          #+#    #+#             */
/*   Updated: 2025/07/31 16:00:01 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *split_tokens(char *input)
{
    int i;

    if (!input)
        return (NULL);
    i = 0;
    while (input[i])
    {
        if (input[i] == ' ' || input[i] == '\t')
            i++;
        else if (input[i] == '|' && (input[i + 1] != '|' || !input[i + 1]))
        {
            ft_printf("Opérateur PIPE trouvé\n");
            i++;
        }
         else if (input[i] == '|' && input[i + 1] == '|')
         {   
            ft_printf("Erreur : opérateur PIPE double\n");
            i++, i++;
         }
        else if (input[i] == '>' && input[i + 1] == '>' && input[i + 1])
            {
                ft_printf("Opérateur APPEND trouvé\n");
                i++, i++;
            }
        else if (input[i] == '>' && input[i + 1] != '>' && input[i + 1])
            {
                ft_printf("Opérateur REDIR_OUT trouve\n");
                i++;
            }
        else if (input[i] == '<' && input[i + 1] == '<' && input[i + 1])
            {
                ft_printf("Opérateur HEREDOC trouvé\n"); 
                i++, i++;
            }
        else if (input[i] == '<' && input[i + 1] != '<' && input[i + 1])
            {
                ft_printf("Opérateur REDIR_IN trouve\n");
                i++;
            }
        else
        {
            /*ft_printf("Caractère à l’index %d : %c\n", i, input[i])*/;
            i++;
        }
    }
    return (NULL);
}