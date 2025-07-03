/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:05:17 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/06/29 18:39:46 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *new_comm()
{
	t_command *new;

	new = malloc(sizeof(t_command));
	new->argv = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
}

int argv_len(char **argv)
{
	int i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

char **argv_add(char ***argv, char *value)
{
	char **new;
	int len;

	len = argv_len(*argv);
	new = realloc((*argv), sizeof(char *) * (len + 2));
	if (!new)
		return (NULL);
	new[len] = value;
	new[len + 1] = NULL;
	return (new);
}

t_redir *new_redir(char *file, t_redir_type type)
{
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->file = file;
	new->type = type;
	new->next = NULL;
	return (new);
}

void redir_add(t_redir **redirs, char *file, t_redir_type type)
{
	t_redir *new;
	t_redir *last;

	new = new_redir(file, type);
	last = (*redirs);
	if ((*redirs) == NULL)
		(*redirs) = new;
	else
	{
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

void add_comm(t_command **comm)
{
	t_command *new;
	t_command *last;

	new = new_comm();
	last = (*comm);
	if ((*comm) == NULL)
		(*comm) = new;
	else
	{
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

t_command *parse(t_command **comm, t_token *token)
{
	t_command *head;

	*comm = new_comm();
	head = (*comm);
	while (token)
	{
		if (token->type == T_WORD)
			(*comm)->argv = argv_add(&(*comm)->argv, token->value);
		else if (token->type == T_REDIR_IN)
		{
			if (token->next)
			{
				redir_add(&(*comm)->redir, token->next->value, R_INPUT);
				token = token->next;
			}
			else
				return (NULL);
		}
		else if (token->type == T_REDIR_OUT)
		{
			if (token->next)
			{
				redir_add(&(*comm)->redir, token->next->value, R_OUTPUT);
				token = token->next;
			}
			else
				return (NULL);
		}
		else if (token->type == T_APPEND)
		{
			if (token->next)
			{
				redir_add(&(*comm)->redir, token->next->value, R_APPEND);
				token = token->next;
			}
			else
				return (NULL);
		}
		else if (token->type == T_HEREDOC)
		{
			if (token->next)
			{
				redir_add(&(*comm)->redir, token->next->value, R_HEREDOC);
				token = token->next;
			}
			else
				return (NULL);
		}
		else if (token->type == T_PIPE)
		{
			if (token->next)
			{
				add_comm(&(*comm));
				(*comm) = (*comm)->next;
			}
			else
				return (NULL);
		}
		token = token->next;
	}
	return (head);
}
