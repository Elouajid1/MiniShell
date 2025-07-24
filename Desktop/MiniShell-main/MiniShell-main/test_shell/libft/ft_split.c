/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <moel-aid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:22:00 by moel-aid          #+#    #+#             */
/*   Updated: 2024/11/11 20:58:21 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	count_words(char const *s, char c)
{
	int		i;
	int		count;
	char	q;

	i = 0;
	count = 0;
	q = 0;
	while (s[i] != 0)
	{
		if (s[i] == c)
		{
			while (s[i] && s[i] == c)
				i++;
		}
		if (s[i] && (s[i] == 39 || s[i] == 34))
		{
			q = s[i];
			i++;
			while (s[i] != 0 && s[i] != q)
				i++;
			if (s[i] && s[i] == q)
			{
				i++;
				count++;
			}
		}
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c && s[i] != 34 && s[i] != 39)
			count++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (count);
}

static int	slen(char const *s, char c)
{
	int		i;
	char	q;

	i = 0;
	while (s[i] && s[i] != c)
	{
		if (s[i] && (s[i] == 34 || s[i] == 39))
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (s[i] == q)
				i++;
		}
		else
		{
			i++;
		}
	}
	return (i);
}

static char	*ft_dup(char const *s, char c)
{
	int		end;
	int		start;
	int		i;
	char	*copy;

	i = 0;
	end = slen(s, c);
	start = 0;
	copy = malloc(sizeof(char) * (end + 1));
	if (!copy)
		return (NULL);
	while (end > start)
	{
		copy[i] = s[start];
		start++;
		i++;
	}
	copy[i] = 0;
	return (copy);
}

static char	**ft_free(char **strs, int count)
{
	while (count-- > 0)
		free(strs[count]);
	free(strs);
	return (NULL);
}

char	**ft_split(char *s, char c)
{
	char **strs;
	int i;

	i = 0;
	if (!s)
		return (NULL);
	strs = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!strs)
		return (NULL);
	while (*s != 0)
	{
		while (*s != 0 && *s == c)
			s++;
		if (*s != 0)
		{
			strs[i] = ft_dup(s, c);
			if (strs[i] == NULL)
				return (ft_free(strs, i));
			i++;
		}
		s += slen(s, c);
	}
	strs[i] = 0;
	return (strs);
}