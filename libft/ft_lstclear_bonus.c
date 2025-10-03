/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:23:29 by pgavel            #+#    #+#             */
/*   Updated: 2025/04/19 11:12:01 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*clear;
	t_list	*temp;

	temp = *lst;
	while (temp)
	{
		clear = temp->next;
		ft_lstdelone(temp, del);
		temp = clear;
	}
	*lst = NULL;
}
