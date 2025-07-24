/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:05:17 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/21 23:41:34 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	argv_len(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

void	handle_redirection(t_cmd *comm, t_token **token)
{
	if ((*token)->type == T_REDIR_IN)
	{
		redir_add(&comm->redir, (*token)->next->value, R_INPUT);
		(*token) = (*token)->next;
	}
	else if ((*token)->type == T_REDIR_OUT)
	{
		redir_add(&comm->redir, (*token)->next->value, R_OUTPUT);
		(*token) = (*token)->next;
	}
	else if ((*token)->type == T_APPEND)
	{
		redir_add(&comm->redir, (*token)->next->value, R_APPEND);
		(*token) = (*token)->next;
	}
	else if ((*token)->type == T_HEREDOC)
	{
		redir_add(&comm->redir, (*token)->next->value, R_HEREDOC);
		(*token) = (*token)->next;
	}
}

t_cmd	*parse(t_cmd **comm, t_token *token)
{
	t_cmd	*head;

	*comm = new_comm();
	head = (*comm);
	while (token)
	{
		if (token->type == T_WORD)
			(*comm)->argv = argv_add((*comm)->argv, token->value);
		else if (token->type == T_REDIR_IN || token->type == T_REDIR_OUT
			|| token->type == T_APPEND || token->type == T_HEREDOC)
			handle_redirection(*comm, &token);
		else if (token->type == T_PIPE)
		{
			add_comm(&(*comm));
			(*comm) = (*comm)->next;
		}
		token = token->next;
	}
	return (head);
}
