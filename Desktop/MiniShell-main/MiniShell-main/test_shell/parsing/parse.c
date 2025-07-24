/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:05:17 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/20 16:04:55 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*new_comm(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	new->argv = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
}

int	argv_len(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

char	**argv_add(char **argv, char *value)
{
	char	**new;
	int		len;
	int		i;
	int		j;

	j = 0;
	i = 0;
	len = argv_len(argv);
	new = malloc(sizeof(char *) * (len + 2));
	if (!new)
		return (NULL);
	if (argv)
	{
		while (argv[i])
		{
			new[i] = argv[i];
			i++;
		}
	}
	new[len] = value;
	new[len + 1] = NULL;
	free(argv);
	return (new);
}

t_redir	*new_redir(char *file, t_redir_type type)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->file = file;
	new->type = type;
	new->next = NULL;
	return (new);
}

void	redir_add(t_redir **redirs, char *file, t_redir_type type)
{
	t_redir	*new;
	t_redir	*last;

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

void	add_comm(t_cmd **comm)
{
	t_cmd	*new;
	t_cmd	*last;

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

t_cmd	*parse(t_cmd **comm, t_token *token)
{
	t_cmd	*head;

	*comm = new_comm();
	head = (*comm);
	while (token)
	{
		if (token->type == T_WORD)
			(*comm)->argv = argv_add((*comm)->argv, token->value);
		else if (token->type == T_REDIR_IN)
		{
			redir_add(&(*comm)->redir, token->next->value, R_INPUT);
			token = token->next;
		}
		else if (token->type == T_REDIR_OUT)
		{
			redir_add(&(*comm)->redir, token->next->value, R_OUTPUT);
			token = token->next;
		}
		else if (token->type == T_APPEND)
		{
			redir_add(&(*comm)->redir, token->next->value, R_APPEND);
			token = token->next;
		}
		else if (token->type == T_HEREDOC)
		{
			redir_add(&(*comm)->redir, token->next->value, R_HEREDOC);
			token = token->next;
		}
		else if (token->type == T_PIPE)
		{
			add_comm(&(*comm));
			(*comm) = (*comm)->next;
		}
		token = token->next;
	}
	return (head);
}
