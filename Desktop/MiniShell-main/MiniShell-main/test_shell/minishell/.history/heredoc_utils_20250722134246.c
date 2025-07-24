/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:28:01 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:26:07 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_last_exit_code;

static char	*append_to_result(char *res, char *value)
{
	char	*tmp;

	tmp = res;
	res = ft_strjoin(res, value);
	free(tmp);
	return (res);
}

static char	*handle_special_vars(char *str, int *i, char *res)
{
	char	*value;

	if (str[*i] == '$')
	{
		value = ft_itoa(getpid());
		res = append_to_result(res, value);
		free(value);
		(*i)++;
	}
	else if (str[*i] == '?')
	{
		value = ft_itoa(g_last_exit_code);
		res = append_to_result(res, value);
		free(value);
		(*i)++;
	}
	return (res);
}

static char	*handle_env_var(char *str, int *i, char *res, t_env *env)
{
	int		j;
	char	*value;
	char	*var;
	t_env	*finded;

	j = *i;
	while (str[*i] && isalnum(str[*i]))
		(*i)++;
	value = ft_substr(str, j, *i - j);
	finded = find_env_node(env, value);
	if (!finded)
	{
		free(value);
		return (NULL);
	}
	var = finded->value;
	if (!var)
		var = "";
	res = append_to_result(res, var);
	free(value);
	return (res);
}

static char	*handle_dollar_expansion(char *str, int *i, char *res, t_env *env)
{
	(*i)++;
	if (!str[*i])
		return (NULL);
	if (str[*i] == '$' || str[*i] == '?')
		return (handle_special_vars(str, i, res));
	else
		return (handle_env_var(str, i, res, env));
}

char	*get_heredoc_expansion(char *str, t_env *env)
{
	int		i;
	char	*res;
	char	temp[2];

	i = 0;
	res = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			res = handle_dollar_expansion(str, &i, res, env);
			if (!res)
				return (NULL);
		}
		else
		{
			temp[0] = str[i];
			temp[1] = '\0';
			res = append_to_result(res, temp);
			i++;
		}
	}
	return (res);
}
