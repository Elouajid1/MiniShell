/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:57:11 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/17 15:57:16 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array || !(*array))
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
}

void	free_redir_list(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir);
		redir = tmp;
	}
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
}

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env_list(shell->env);
	if (shell->cmds)
		free_command(cmd);
	free(shell);
	shell->env = NULL;
	shell->cmds = NULL;
}
