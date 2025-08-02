/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:49:14 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/30 12:51:27 by mel-ouaj         ###   ########.fr       */
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

int	count_fields(char **strs, t_shell *shell)
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
		expanded = expand_str(strs[i], shell);
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

void	fields_expand(t_args *args, t_shell *shell)
{
	char	**tmp;
	char	**iter;
	char	*expanded;

	expanded = expand_str(args->strs[args->i], shell);
	if (check_sp(expanded))
	{
		tmp = ft_split1(expanded, 32);
		iter = tmp;
		while (*iter)
		{
			args->res[args->j] = ft_strdup(*iter);
			args->j++;
			iter++;
		}
		free_array(tmp);
	}
	else
	{
		args->res[args->j] = ft_strdup(expanded);
		args->j++;
	}
	free(expanded);
	args->i++;
}

void	check_heredoc(t_args *args)
{
	args->res[args->j++] = ft_strdup(args->strs[args->i++]);
	if (args->strs[args->i])
		args->res[args->j++] = ft_strdup(args->strs[args->i++]);
}

int	init_args(t_args *args, char **strs, t_shell *shell)
{
	args->strs = strs;
	args->res = malloc(sizeof(char *) * (count_fields(strs, shell) + 1));
	if (!args->res)
		return (0);
	args->i = 0;
	args->j = 0;
	return (1);
}
