/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:58:31 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:25:34 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	backup_fds[2];

	if (cmd->redir)
	{
		if (backup_stdio(backup_fds) == -1)
			return (1);
		if (setup_redirections(cmd->redir) == -1)
		{
			restore_stdio(backup_fds);
			return (1);
		}
	}
	status = handle_builtin_logic(cmd, shell);
	if (cmd->redir)
		restore_stdio(backup_fds);
	return (status);
}

void	unset_variable(t_shell *shell, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = shell->env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static void	populate_env_array(t_env *env, char **envp, int count)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (current && i < count)
	{
		if (current->value)
		{
			envp[i] = create_env_string(current);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	int		count;

	count = count_env_with_values(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	populate_env_array(env, envp, count);
	return (envp);
}

int	print_export_list(t_env *env, t_shell *shell)
{
	t_env	*current;

	current = env;
	if (!shell->env_flag)
	{
		while (current)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(current->key, STDOUT_FILENO);
			if (current->value)
			{
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(current->value, STDOUT_FILENO);
				ft_putchar_fd('"', STDOUT_FILENO);
			}
			ft_putchar_fd('\n', STDOUT_FILENO);
			current = current->next;
		}
	}
	else
		print_export_list1(env);
	return (0);
}
