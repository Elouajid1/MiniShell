/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:39:09 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/21 23:40:44 by mel-ouaj         ###   ########.fr       */
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

char	**argv_add(char **argv, char *value)
{
	char	**new;
	int		len;
	int		i;

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
	new[len] = ft_strdup(value);
	new[len + 1] = NULL;
	free(argv);
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
