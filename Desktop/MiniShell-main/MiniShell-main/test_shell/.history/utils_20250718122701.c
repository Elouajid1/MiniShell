/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:01:42 by moel-aid          #+#    #+#             */
/*   Updated: 2025/05/28 17:28:09 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern int	g_last_exit_code;

void	print_env_values(t_env *current)
{
	if (current->show_in_export)
	{
		ft_putstr_fd(current->key, 1);
		if (current->value)
		{
			ft_putchar_fd('=', 1);
			ft_putchar_fd(34, 1);
		}
		ft_putstr_fd(current->value, 1);
		if (!current->value)
			write(1, "\n", 1);
		if (current->value)
		{
			ft_putchar_fd(34, 1);
			write(1, "\n", 1);
		}
	}
}

int	print_export_list1(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		print_env_values(current);
		current = current->next;
	}
	return (0);
}

char	*find_executable_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*full_path;
	char	*temp;

	if (!cmd || !find_env_key(shell->env, "PATH"))
		return (NULL);
	paths = ft_split(find_env_key(shell->env, "PATH"), ':');
	if (!paths)
		return (NULL);
	shell->i = -1;
	while (paths[++shell->i])
	{
		temp = ft_strjoin(paths[shell->i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_array(paths);
	shell->flag = 2;
	return (NULL);
}

char	*get_executable_path(char *cmd, t_shell *shell)
{
	char	*tmp;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
		{
			shell->flag = 1;
			return (NULL);
		}
	}
	tmp = find_executable_path(cmd, shell);
	if (!tmp)
		shell->flag = 2;
	printf("%s\n", )
	return (tmp);
}
