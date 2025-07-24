/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:46:20 by isallali          #+#    #+#             */
/*   Updated: 2025/07/22 11:26:47 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	skip_q(char *s, int i)
{
	char	q;

	q = s[i];
	i++;
	while (s[i] && s[i] != q)
		i++;
	if (s[i] == q)
		i++;
	return (i);
}

int	slen(char *s, char c)
{
	int		i;

	i = 0;
	if (s[i] && is_operator(s[i]))
	{
		if (is_double_operator(s[i], s[i + 1]))
			return (2);
		else
			return (1);
	}
	while (s[i] && s[i] != c && !is_operator(s[i]))
	{
		if (s[i] && (s[i] == 34 || s[i] == 39))
			i = skip_q(s, i);
		else
			i++;
	}
	return (i);
}

char	*ft_dup(char *s, char c)
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

char	**ft_free(char **strs, int count)
{
	while (count-- > 0)
		free(strs[count]);
	free(strs);
	return (NULL);
}

char	**ft_split1(char *s, char c)
{
	char	**strs;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	strs = malloc(sizeof(char *) * (count_words(s, c) + 2));
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
