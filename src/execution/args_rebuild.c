/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_rebuild.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:01:16 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/05 17:58:45 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_total_args(char ***all_splits, int count)
{
	int	total;
	int	i;
	int	j;

	total = 0;
	i = 0;
	while (i < count)
	{
		j = 0;
		while (all_splits[i][j])
		{
			total++;
			j++;
		}
		i++;
	}
	return (total);
}

void	build_new_args(char **new_args, char ***splits, int count)
{
	int	idx;
	int	i;
	int	j;

	idx = 0;
	i = 0;
	while (i < count)
	{
		j = 0;
		while (splits[i][j])
		{
			new_args[idx++] = ft_strdup(splits[i][j]);
			j++;
		}
		i++;
	}
	new_args[idx] = NULL;
}

void	free_splits(char ***splits, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count)
	{
		j = 0;
		while (splits[i][j])
		{
			free(splits[i][j]);
			j++;
		}
		free(splits[i]);
		i++;
	}
	free(splits);
}
