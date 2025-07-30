/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:06:04 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/29 22:32:12 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token **token)
{
	t_token	*tmp;

	if (!token || !(*token))
		return ;
	while (*token)
	{
		tmp = (*token)->next;
		if ((*token)->value)
			free((*token)->value);
		free((*token));
		(*token) = tmp;
	}
}

void	free_redir(t_redir **redir)
{
	t_redir	*tmp;

	if (!redir || !(*redir))
		return ;
	while (*redir)
	{
		tmp = (*redir)->next;
		free((*redir));
		(*redir) = tmp;
	}
}

void	free_command(t_cmd **comm)
{
	int		i;
	t_cmd	*tmp;

	if (!comm || !(*comm))
		return ;
	while (*comm)
	{
		i = 0;
		while ((*comm)->argv[i])
		{
			free((*comm)->argv[i]);
			i++;
		}
		free((*comm)->argv);
		free_redir(&(*comm)->redir);
		tmp = (*comm)->next;
		free(*comm);
		(*comm) = tmp;
	}
}

void	free_args(t_args *args)
{
	if (args->res)
		free_array(args->res);
	if (args->strs)
		free_array(args->res);
	free(args);
}
