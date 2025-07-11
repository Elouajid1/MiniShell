/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:58:31 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/24 15:58:36 by moel-aid         ###   ########.fr       */
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

int	export_variable(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	char	*equal_sign;
	t_env	*existing;
	t_env	*new_node;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	existing = find_env_node(shell->env, key);
	if (existing)
	{
		if (value)
		{
			free(existing->value);
			existing->value = value;
		}
		free(key);
	}
	else
	{
		new_node = create_env_node(key, value);
		if (!new_node)
		{
			free(key);
			free(value);
			return (1);
		}
		add_env_node(&shell->env, new_node);
		free(key);
	}
	if (existing && (value != existing->value))
		free(value);
	return (0);
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

char	**env_to_array(t_env *env)
{
	char	**envp;
	char	*temp;
	int		count;
	int		i;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		if (current->value)
			count++;
		current = current->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current && i < count)
	{
		if (current->value)
		{
			temp = ft_strjoin(current->key, "=");
			envp[i] = ft_strjoin(temp, current->value);
			free(temp);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	print_export_list(t_env *env)
{
	t_env *current;

	current = env;
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
	return (0);
}