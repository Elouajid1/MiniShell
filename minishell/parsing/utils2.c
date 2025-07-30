/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:49:08 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/22 11:05:48 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	handle_q(char *s, int *i)
{
	char	q;

	q = s[*i];
	(*i)++;
	while (s[*i] != 0 && s[*i] != q)
		(*i)++;
	if (s[*i] && s[*i] == q)
		(*i)++;
	return (1);
}

int	handle_op(char *s, int *i)
{
	if (s[*i] && is_operator(s[*i]))
	{
		if (is_double_operator(s[*i], s[(*i) + 1]))
			(*i) += 2;
		else
			(*i)++;
		return (1);
	}
	return (1);
}

int	count_words(char *s, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i] != 0)
	{
		while (s[i] && s[i] == c)
			i++;
		if (handle_op(s, &i))
			count++;
		else if (s[i] && (s[i] == 39 || s[i] == 34))
			count += handle_q(s, &i);
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c && !is_operator(s[i]) && s[i] != 34 && s[i] != 39)
			count++;
		while (s[i] != '\0' && !is_operator(s[i]) && s[i] != c)
			i++;
	}
	return (count);
}
