/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:01:42 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:27:11 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_last_exit_code;

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

int	count_dots(char *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		if (cmd[i] == '.')
			count++;
		if (cmd[i + 1] == '.' && cmd[i] == '.')
			return (-1);
		i++;
	}
	return (count);
}

char	*handle_dot_case(t_shell *shell, char *cmd)
{
	char	*full_path;

	if (cmd[0] == ':')
	{
		shell->flag = 0;
		return (NULL);
	}
	if (shell->cmds->argv[2] && !shell->cmds->argv[3])
	{
		if (shell->cmds->argv[0] == '.' && shell->cmds->argv[1] == '')
		if (count_dots(cmd) > 2)
		{
			ft_putstr_fd("minishell: .: .: is a directory\n", STDERR_FILENO);
			shell->flag = 3;
			return (NULL);
		}
	}
	if (count_dots(cmd) == 1)
	{
		ft_putstr_fd("minishell: .: filename argument required\n",
			STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
		shell->flag = -2;
		return (NULL);
	}
	full_path = search_in_path(cmd, shell);
	if (!full_path)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 20);
		shell->flag = 2;
		return (NULL);
	}
	return (full_path);
}

char	*get_executable_path(char *cmd, t_shell *shell)
{
	char *full_path;

	shell->flag = 0;
	if (!cmd || !cmd[0])
	{
		shell->flag = 1;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == -1)
			return (no_such_file_error(cmd, shell));
		if (access(cmd, X_OK) == -1)
			return (permission_error(cmd, shell));
		full_path = ft_strdup(cmd);
		if (!full_path)
		{
			write(2, "Memory allocation failed\n", 25);
			shell->flag = 1;
			return (NULL);
		}
		return (full_path);
	}
	return (handle_dot_case(shell, cmd));
}