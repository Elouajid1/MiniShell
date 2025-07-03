/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:04:20 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/06/28 17:04:32 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int token_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (T_PIPE);
	else if (ft_strcmp(str, "<") == 0)
		return (T_REDIR_IN);
	else if (ft_strcmp(str, ">") == 0)
		return (T_REDIR_OUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (T_APPEND);
	else if (ft_strcmp(str, "<<") == 0)
		return (T_HEREDOC);
	else
		return (T_WORD);
}

t_token *ft_lstnew(char *content)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = content;
	new->type = token_type(content);
	new->next = NULL;
	return (new);
}

void ft_lstadd_back(t_token **tokens, t_token *token)
{
	t_token *last;

	if (!tokens || !token)
		return;
	last = (*tokens);
	if ((*tokens) == NULL)
		(*tokens) = token;
	else
	{
		while (last->next != NULL)
		{
			last = last->next;
		}
		last->next = token;
	}
}

void tokenize(t_token **token, char **strs)
{
	int i;
	t_token *new;

	i = 0;
	while (strs[i])
	{
		new = ft_lstnew(strs[i]);
		ft_lstadd_back(token, new);
		i++;
	}
}