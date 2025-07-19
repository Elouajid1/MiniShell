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

char	*get_heredoc_expansion(char *str, t_env *env)
{
	int i;
	int j;
	char *value;
	char *var;
	char *res;
	char *tmp;
	t_env *finded;
	char temp[2];

	i = 0;
	j = 0;
	res = ft_strdup("");
	var = NULL;
	value = NULL;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			if (!str[i])
				return (NULL);
			if (str[i] && str[i] == '$')
			{
				tmp = res;
				value = ft_itoa(getpid());
				res = ft_strjoin(res, value);
				free(value);
				free(tmp);
				i++;
			}
			else if (str[i] && str[i] == '?')
			{
				tmp = res;
				value = ft_itoa(g_last_exit_code);
				res = ft_strjoin(res, value);
				free(value);
				free(tmp);
				i++;
			}
			else
			{
				j = i;
				while (str[i] && isalnum(str[i]))
					i++;
				value = ft_substr(str, j, i - j);
				finded = find_env_node(env, value);
				if (!finded)
					return (NULL);
				var = finded->value;
				if (!var)
					var = "";
				tmp = res;
				res = ft_strjoin(res, var);
				free(tmp);
				free(value);
			}
		}
		else if (str[i] == '$')
		{
			tmp = res;
			res = ft_strjoin(res, "$");
			free(tmp);
			i++;
		}
		else
		{
			temp[0] = str[i];
			temp[1] = '\0';
			tmp = res;
			res = ft_strjoin(res, temp);
			free(tmp);
			i++;
		}
	}
	return (res);
}