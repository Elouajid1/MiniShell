/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <moel-aid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:17:55 by moel-aid          #+#    #+#             */
/*   Updated: 2024/11/12 18:14:21 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static char	*trimmed(const char *s1, size_t start, size_t len)
{
	char	*tmp;
	size_t	i;

	if (!s1)
		return (NULL);
	if (len <= 0 || start >= ft_strlen(s1))
		return (ft_strdup(""));
	tmp = ft_calloc(len + 1, sizeof(char));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < len)
	{
		tmp[i] = s1[start + i];
		i++;
	}
	return (tmp);
}

static int	trim(const char	*set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	i;
	int	j;

	if (!set || !s1)
		return (NULL);
	i = 0;
	j = ft_strlen(s1) - 1;
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	while (trim(set, s1[i]))
		i++;
	while (trim(set, s1[j]))
		j--;
	return (trimmed(s1, i, j - (i - 1)));
}
