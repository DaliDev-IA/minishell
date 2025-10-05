/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_filter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:22:19 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 17:58:41 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_non_empty_args(char **args)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

char	**filter_empty_args(char **args)
{
	char	**result;
	int		i;
	int		j;
	int		count;

	count = count_non_empty_args(args);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
			result[j++] = args[i];
		else
			free(args[i]);
		i++;
	}
	result[j] = NULL;
	free(args);
	return (result);
}
