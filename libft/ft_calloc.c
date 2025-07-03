/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 09:25:39 by mel-ouaj          #+#    #+#             */
/*   Updated: 2024/11/12 15:40:08 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*str;

	if (count != 0 && size > 18446744073709551615UL / count)
		return (NULL);
	str = malloc(sizeof(char) * (count * size));
	if (!str)
		return (NULL);
	ft_bzero(str, count * size);
	return (str);
}
