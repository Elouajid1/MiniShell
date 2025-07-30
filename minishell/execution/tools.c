/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:25:16 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:02:09 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

int	wait_for_children(t_shell *shell)
{
	shell->i = -1;
	shell->status = 0;
	shell->printed = false;
	while (++shell->i < shell->pipeline_count)
	{
		waitpid(shell->pipeline_pids[shell->i], &shell->status, 0);
		if (WIFSIGNALED(shell->status))
		{
			shell->sig = WTERMSIG(shell->status);
			if (shell->sig == SIGINT)
				shell->last_status = 130;
			else if (shell->sig == SIGQUIT && !shell->printed)
			{
				shell->printed = true;
				shell->last_status = 131;
			}
			else
				shell->last_status = 128 + shell->sig;
		}
		else if (WIFEXITED(shell->status))
			shell->last_status = WEXITSTATUS(shell->status);
	}
	return (shell->last_status);
}

int	print_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->show_in_env && current->value)
		{
			ft_putstr_fd(current->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(current->value, 1);
		}
		current = current->next;
	}
	return (0);
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (key)
		node->key = strdup(key);
	else
		node->key = NULL;
	if (value)
		node->value = strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	add_defaults_env(t_env **env)
{
	t_env	*pwd;
	t_env	*shlvl;
	t_env	*oldpwd;
	char	cwd[1024];

	if (!env)
		return ;
	getcwd(cwd, sizeof(cwd));
	pwd = new_env_node("PWD", cwd);
	shlvl = new_env_node("SHLVL", "2");
	oldpwd = new_env_node("_", "/usr/bin/env");
	if (!pwd || !shlvl || !oldpwd)
		return ;
	*env = pwd;
	pwd->next = shlvl;
	shlvl->next = oldpwd;
}

void	close_all_pipes(int pipes[][2], int pipe_count)
{
	int	i;

	i = -1;
	while (++i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
