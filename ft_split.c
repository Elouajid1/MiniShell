/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:46:20 by isallali          #+#    #+#             */
/*   Updated: 2025/07/09 13:51:30 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_double_operator(char c, char next)
{
	if ((c == '>' && next == '>') || (c == '<' && next == '<'))
		return (1);
	return (0);
}

static int count_words(char const *s, char c)
{
	int i;
	int count;
	char q;

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
		if (s[i] && is_operator(s[i]))
		{
			count++;
			if (is_double_operator(s[i], s[i + 1]))
				i += 2;
			else
				i++;
		}
		else if (s[i] && (s[i] == 39 || s[i] == 34))
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
		if (s[i] && s[i] != c && !is_operator(s[i]) && s[i] != 34 && s[i] != 39)
			count++;
		while (s[i] != '\0' && !is_operator(s[i]) && s[i] != c)
			i++;
	}
	return (count);
}

static int slen(char const *s, char c)
{
	int i;
	char q;

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

static char *ft_dup(char const *s, char c)
{
	int end;
	int start;
	int i;
	char *copy;

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

static char **ft_free(char **strs, int count)
{
	while (count-- > 0)
		free(strs[count]);
	free(strs);
	return (NULL);
}

char	**ft_split(char const *s, char c)
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

// int main ()
// {
// 	int i = 0;
// 	char *str = "|gf jkng |\"fdkg\"|";
// 	char **strs = ft_split(str, 32);
	
// 	while (strs[i])
// 	{
// 		printf("%d\n", slen(strs[i], 32));
// 		i++;
// 	}
// 	// printf("%d\n", count_words(str, 32));
// }