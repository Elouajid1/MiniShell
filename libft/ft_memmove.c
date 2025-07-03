/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:49:05 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/06/28 17:35:32 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_cpy;
	unsigned const char	*src_cpy;

	if ((!src && !dst))
		return (NULL);
	src_cpy = (unsigned const char *)src;
	dst_cpy = (unsigned char *)dst;
	if (src > dst)
	{
		ft_memcpy(dst, src, n);
	}
	else
	{
		while (n)
		{
			dst_cpy[n - 1] = src_cpy[n - 1];
			n--;
		}
	}
	return (dst);
}
