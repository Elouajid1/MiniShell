/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:27:04 by mel-ouaj          #+#    #+#             */
/*   Updated: 2024/11/11 10:56:15 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len(long n)
{
	int	i;

	i = 0;
	if (n <= 0)
	{
		if (n == 0)
			return (1);
		n *= -1;
		i++;
	}
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static void	filling(char *str, long nb, size_t i)
{
	str[i--] = '\0';
	if (nb == 0)
	{
		str[0] = '0';
		return ;
	}
	while (nb > 0)
	{
		str[i] = (nb % 10) + '0';
		nb /= 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	long	nb;
	size_t	i;
	char	*str;

	i = len(n);
	nb = n;
	str = malloc(sizeof (char) * (i + 1));
	if (!str)
		return (0);
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
		filling(str, nb, i);
	}
	else
		filling(str, nb, i);
	return (str);
}
