/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:57:11 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/20 21:45:34 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
	free(env);
	env = NULL;
}

void	free_redir_list(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
		free(redir);
		redir = tmp;
	}
	free(redir);
	redir = NULL;
}

void	free_cmd_list(t_cmd *cmds)
{
	t_cmd	*tmp;

	if (!cmds)
		return ;
	while (cmds)
	{
		tmp = cmds->next;
		free_array(cmds->argv);
		free_redir_list(cmds->redir);
		free(cmds);
		cmds = tmp;
	}
	free(cmds);
	cmds = NULL;
}

void	cleanup_shell(t_shell *shell, bool flag)
{
	if (shell == NULL)
		return ;
	if (shell->env && flag)
		free_env_list(shell->env);
	if (shell->token)
		free_tokens(&(shell->token));
	if (shell->cmds)
		free_cmd_list(shell->cmds);
}
