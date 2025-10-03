/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:23:52 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 22:23:53 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	*processed_input;

	processed_input = preprocess_input(input);
	if (!processed_input)
		return (NULL);
	tokens = NULL;
	tokens = process_tokens(processed_input, tokens);
	if (!tokens)
	{
		free(processed_input);
		return (NULL);
	}
	free(processed_input);
	return (tokens);
}
