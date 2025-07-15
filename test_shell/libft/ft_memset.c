/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <moel-aid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:21:17 by moel-aid          #+#    #+#             */
/*   Updated: 2024/11/12 19:00:52 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *s, int a, size_t len)
{
	unsigned int	i;
	unsigned char	*tmp;

	i = 0;
	tmp = (unsigned char *) s;
	while (i < len)
	{
		tmp[i] = (unsigned char) a;
		i++;
	}
	return (s);
}
