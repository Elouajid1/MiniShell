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

void	print_env_values(t_env *current, t_shell *shell)
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
		if(!current->value)
			write(1, "\n", 1);
		if (current->value)
		{
			ft_putchar_fd(34, 1);
			write(1, "\n", 1);
		}
	}
}

int	print_export_list1(t_env *env, t_shell *shell)
{
	t_env *current;
	
	current = env;
	while (current)
	{
		print_env_values(current, shell);
		current = current->next;
	}
	return (0);
}

bool	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

char	*find_executable_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd || !find_env_key(shell->env, "PATH"))
		return (NULL);
	paths = ft_split(find_env_key(shell->env, "PATH"), ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
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
	return (tmp);
}
