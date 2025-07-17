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

char    *find_executable_path(char *cmd, t_shell *shell)
{
    char    **paths;
    char    *full_path;
    char    *temp;
    char    *env_path; // Store find_env_key result

    if (!cmd)
        return (NULL);
    
    env_path = find_env_key(shell->env, "PATH");
    if (!env_path) // Handle PATH not found
        return (NULL);

    paths = ft_split(env_path, ':');
    if (!paths)
        return (NULL);

    shell->i = -1;
    while (paths[++shell->i])
    {
        temp = ft_strjoin(paths[shell->i], "/");
        if (!temp) // Allocation failure for temp
        {
            free_array(paths); // Free paths array
            return (NULL);     // Indicate failure
        }

        full_path = ft_strjoin(temp, cmd);
        if (!full_path) // Allocation failure for full_path
        {
            free(temp); // Free temp from this iteration
            // Consider if you want to free paths and return NULL here,
            // or continue to the next path. Continuing is usually fine
            // as it's not a leak for *this* iteration's resources.
            continue; // Try next path
        }

        if (access(full_path, X_OK) == 0)
        {
            free_array(paths);
            free(temp); // temp was created in this iteration and needs freeing
            return (full_path); // Returned, caller frees
        }
        free(full_path); // Free full_path if not executable
        free(temp);      // Free temp for current iteration
    }
    free_array(paths); // Free paths if loop finishes without finding anything
    shell->flag = 2;
    return (NULL);
}

// char	*get_executable_path(char *cmd, t_shell *shell)
// {
// 	char	*tmp;

// 	if (!cmd)
// 		return (NULL);
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, X_OK) == 0)
// 			return (ft_strdup(cmd));
// 		else
// 		{
// 			shell->flag = 1;
// 			return (NULL);
// 		}
// 	}
// 	tmp = find_executable_path(cmd, shell);
// 	if (!tmp)
// 		shell->flag = 2;
// 	return (tmp);
// }
