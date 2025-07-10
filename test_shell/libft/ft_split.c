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

// static void	vars_initilization(size_t *i, int *j, int *s_word)
// {
// 	*i = 0;
// 	*j = 0;
// 	*s_word = -1;
// }

// static void	*ft_free(char **strs, int count)
// {
// 	int	i;

// 	i = 0;
// 	while (i < count)
// 	{
// 		free(strs[i]);
// 		i++;
// 	}
// 	free(strs);
// 	return (NULL);
// }

// static char	*fill_word(const char *str, int start, int end)
// {
// 	char	*word;
// 	int		i;

// 	i = 0;
// 	word = malloc((end - start + 1) * sizeof(char));
// 	if (!word)
// 		return (NULL);
// 	while (start < end)
// 	{
// 		word[i] = str[start];
// 		i++;
// 		start++;
// 	}
// 	word[i] = 0;
// 	return (word);
// }

// static int	count_strings(const char *str, char c)
// {
// 	int	i;
// 	int	count;

// 	count = 0;
// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		while (str[i] != '\0' && str[i] == c)
// 			i++;
// 		if (str[i] != '\0')
// 			count++;
// 		while (str[i] != '\0' && !(str[i] == c))
// 			i++;
// 	}
// 	return (count);
// }

// char	**ft_split(const char *s, char c)
// {
// 	char	**res;
// 	size_t	i;
// 	int		j;
// 	int		s_word;

// 	if (!s)
// 		return (NULL);
// 	vars_initilization(&i, &j, &s_word);
// 	res = ft_calloc((count_strings(s, c) + 1), sizeof(char *));
// 	if (!res)
// 		return (NULL);
// 	while (i <= ft_strlen(s))
// 	{
// 		if (s[i] != c && s_word < 0)
// 			s_word = i;
// 		else if ((s[i] == c || i == ft_strlen(s)) && s_word >= 0)
// 		{
// 			res[j] = fill_word(s, s_word, i);
// 			if (!(res[j++]))
// 				return (ft_free(res, j));
// 			s_word = -1;
// 		}
// 		i++;
// 	}
// 	return (res);
// }


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

static int slen(char const *s, char c)
{
	int i;
	char q;

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