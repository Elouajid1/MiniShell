/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:36:35 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/29 17:19:54 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_q(char c, int *double_q, int *single_q)
{
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
	if (str[*i] >= 48 && str[*i] <= 57)
		(*i)++;
	else
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
	value = ft_substr(str, j, *i - j);
	finded = find_env_node(env, value);
	if (!finded || !finded->value)
		var = "";
	else
		var = finded->value;
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

void	q_rremoval(t_token **token, t_shell *shell)
{
	t_token	*head;
	char	*new;

	shell->quote_flag = 0;
	head = (*token);
	while (head)
	{
		if (head->type == T_HEREDOC)
			head = head->next;
		else if (head->type == T_WORD && head->value)
		{
			new = rm_quotes(head->value);
			if (new)
			{
				free(head->value);
				head->value = new;
			}
			else
			{
				shell->quote_flag = 1;
				return ;
			}
		}
		head = head->next;
	}
}
