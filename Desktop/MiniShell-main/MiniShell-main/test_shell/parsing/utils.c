/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:36:35 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/20 01:10:01 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_q(char c, int *double_q, int *single_q)
{
	int	i;

	i = 0;
	if (c == 34 && !(*single_q))
	{
		if (!(*double_q))
			*double_q = 1;
		else
			*double_q = 0;
	}
	else if (c == 39 && !(*double_q))
	{
		if (!(*single_q))
			*single_q = 1;
		else
			*single_q = 0;
	}
}

char	*expand_dollar(char *res)
{
	char	*tmp;

	tmp = res;
	res = ft_strjoin(tmp, "$");
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}

char	*expand_value(char *res, char *str, int *i, t_env *env)
{
	int		j;
	char	*value;
	char	*var;
	char	*tmp;
	t_env	*finded;

	j = *i;
	while (str[*i] && isalnum(str[*i]))
		(*i)++;
	value = ft_substr(str, j, *i - j);
	finded = find_env_node(env, value);
	if (!finded)
		var = "";
    else if (!finded->value)
        var = "";
    else
	    var = finded->value;
	if (!var)
		var = "";
	tmp = res;
	res = ft_strjoin(tmp, var);
	free(tmp);
	free(value);
	if (!res)
		return (NULL);
	return (res);
}

char	*append_char(char *res, char c)
{
	char	temp[2];
	char	*tmp;

	temp[0] = c;
	temp[1] = '\0';
	tmp = res;
	res = ft_strjoin(tmp, temp);
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}

void	q_rremoval(t_token **token)
{
	t_token	*head;

	head = (*token);
	while (head)
	{
		if (head->type == T_HEREDOC)
			head = head->next;
		else if (head->type == T_WORD)
			head->value = rm_quotes(head->value);
		head = head->next;
	}
}
