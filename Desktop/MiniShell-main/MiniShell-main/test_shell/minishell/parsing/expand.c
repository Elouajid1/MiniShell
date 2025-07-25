/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:54:55 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/22 11:26:39 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

int	q_handle(char c, int *double_q, int *single_q)
{
	if (c == 34 && !(*single_q))
	{
		*double_q = !*double_q;
		return (1);
	}
	else if (c == 39 && !(*double_q))
	{
		*single_q = !*single_q;
		return (1);
	}
	return (0);
}

char	*rm_quotes(char *str)
{
	int		i;
	int		j;
	int		single_q;
	int		double_q;
	char	*res;

	i = 0;
	j = 0;
	single_q = 0;
	double_q = 0;
	if (!str)
		return (NULL);
	res = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (q_handle(str[i], &single_q, &double_q))
			i++;
		else
			res[j++] = str[i++];
	}
	if (single_q == 1 || double_q == 1)
		return (free(res), printf("Syntax Error! near quotes\n"), NULL);
	res[j] = 0;
	return (res);
}

char	*expand_exit(char *res)
{
	char	*tmp;
	char	*value;

	tmp = res;
	value = ft_itoa(g_last_exit_code);
	if (!value)
	{
		free(tmp);
		return (NULL);
	}
	res = ft_strjoin(tmp, value);
	free(tmp);
	free(value);
	if (!res)
		return (NULL);
	return (res);
}

char	*expand_zero(char *res)
{
	char *tmp;

	tmp = res;
	res = ft_strjoin(tmp, "minishell");
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}

char	*handle_dollar(char *res, char *str, int *i, t_env *env)
{
	if (str[*i] && str[*i] == '$')
	{
		(*i)++;
		return (expand_dollar(res));
	}
	else if (str[*i] && str[*i] == '?')
	{
		(*i)++;
		return (expand_exit(res));
	}
	else if (str[*i] && str[*i] == '0')
	{
		(*i)++;
		return (expand_zero(res));
	}
	else
		return (expand_value(res, str, i, env));
}

char	*expand_str(char *str, t_env *env)
{
	char	*res;

	int (i), (single_q), (double_q);
	i = 0;
	single_q = 0;
	double_q = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (str[i])
	{
		check_q(str[i], &double_q, &single_q);
		if (str[i] == '$' && !single_q)
		{
			i++;
			if (!str[i])
			{
				return (append_char(res, '$'));
			}
			res = handle_dollar(res, str, &i, env);
		}
		else
			res = append_char(res, str[i++]);
	}
	return (res);
}
