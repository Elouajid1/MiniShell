/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:30:51 by mel-ouaj          #+#    #+#             */
/*   Updated: 2024/11/12 15:19:33 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	s1len;
	size_t	s2len;

	if (!s1 && !s2)
		return (0);
	if (s1)
		s1len = ft_strlen(s1);
	else
		s1len = 0;
	if (s2)
		s2len = ft_strlen(s2);
	else
		s2len = 0;
	str = ft_calloc((s1len + s2len + 1), sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, s1len);
	ft_memcpy(str + s1len, s2, s2len);
	return (str);
}
