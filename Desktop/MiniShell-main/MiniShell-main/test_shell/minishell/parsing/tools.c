/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:49:14 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/29 15:53:09 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_sp(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == 32)
			return (1);
		i++;
	}
	return (0);
}

int	count_fields(char **strs, t_env *env)
{
	char	*expanded;
	char	**fields;
	int		count;
	int		i;

	i = 0;
	count = 0;
	if (!strs)
		return (0);
	while (strs[i])
	{
		fields = NULL;
		expanded = expand_str(strs[i], env);
		if (check_sp(expanded))
		{
			fields = ft_split1(expanded, 32);
			count += argv_len(fields);
		}
		else
			count++;
		free(expanded);
		free_array(fields);
		i++;
	}
	return (count);
}

void	fields_expand(char **strs, char **res, int *i, int *j, t_env *env)
{
	char	**tmp;
	char	**iter;
	char	*expanded;

	expanded = expand_str(strs[*i], env);
	if (check_sp(expanded))
	{
		tmp = ft_split1(expanded, 32);
		iter = tmp;
		while (*iter)
		{
			res[*j] = ft_strdup(*iter);
			(*j)++;
			iter++;
		}
		free_array(tmp);
	}
	else
	{
		res[*j] = ft_strdup(expanded);
		(*j)++;
	}
	free(expanded);
	(*i)++;
}

char	**expander(char **strs, t_env *env)
{
	char	**res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * (count_fields(strs, env) + 1));
	if (!res)
		return (NULL);
	while (strs[i])
	{
		if (ft_strcmp("<<", strs[i]) == 0)
		{
			res[j++] = ft_strdup(strs[i++]);
			if (strs[i])
				res[j++] = ft_strdup(strs[i++]);
			continue ;
		}
		else
			fields_expand(strs, res, &i, &j, env);
	}
	res[j] = NULL;
	return (res);
}

char	*expand_zero(char *res)
{
	char	*tmp;

	tmp = res;
	res = ft_strjoin(tmp, "minishell");
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}
