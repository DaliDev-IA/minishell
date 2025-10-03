/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:58:53 by pgavel            #+#    #+#             */
/*   Updated: 2025/04/19 11:11:42 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calculate_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
	}
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	fill_number(char *str, int n, int len)
{
	unsigned int	nbr;
	int				i;

	if (n < 0)
	{
		str[0] = '-';
		nbr = (unsigned int)(-n);
	}
	else
		nbr = (unsigned int)n;
	str[len] = '\0';
	if (nbr == 0)
	{
		str[0] = '0';
		return ;
	}
	i = len - 1;
	while (nbr > 0)
	{
		str[i] = (nbr % 10) + '0';
		nbr = nbr / 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;

	len = calculate_len(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
	{
		return (NULL);
	}
	fill_number(str, n, len);
	return (str);
}
