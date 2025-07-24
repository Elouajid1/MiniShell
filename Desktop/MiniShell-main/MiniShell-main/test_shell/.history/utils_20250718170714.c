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


// Helper function to search in PATH directories
char *search_in_path(char *cmd, t_shell *shell)
{
	char *path_env;
	char *path_copy;
	char *dir;
	char *full_path;
	struct stat st;
	int i, start, len;
	
	// Get PATH environment variable
	path_env = get_env_value(shell->env, "PATH"); // Your existing function
	if (!path_env)
		return (NULL);
	
	// Make a copy of PATH to work with
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	
	// Parse PATH manually (since strtok is not authorized)
	i = 0;
	start = 0;
	while (path_copy[i])
	{
		if (path_copy[i] == ':' || path_copy[i + 1] == '\0')
		{
			// Calculate directory length
			len = (path_copy[i] == ':') ? i - start : i - start + 1;
			
			// Skip empty directories
			if (len == 0)
			{
				start = i + 1;
				i++;
				continue;
			}
			
			// Create directory string
			dir = malloc(len + 1);
			if (!dir)
			{
				free(path_copy);
				return (NULL);
			}
			ft_strncpy(dir, &path_copy[start], len); // Use your ft_strncpy
			dir[len] = '\0';
			
			// Construct full path
			full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
			if (!full_path)
			{
				free(dir);
				free(path_copy);
				return (NULL);
			}
			
			ft_strcpy(full_path, dir); // Use your ft_strcpy
			ft_strcat(full_path, "/"); // Use your ft_strcat
			ft_strcat(full_path, cmd);
			
			// Check if file exists and is executable
			if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode) && access(full_path, X_OK) == 0)
			{
				free(dir);
				free(path_copy);
				return (full_path);
			}
			
			free(full_path);
			free(dir);
			start = i + 1;
		}
		i++;
	}
	
	free(path_copy);
	return (NULL);
}

int	count_dots(char *cmd, t_shell *shell)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while(cmd[i])
	{
		if (cmd[i] == '.' && cmd[i + 1] != '.')
		if (cmd[i] == '.')
			count++;
		if (cmd[i + 1] == '.' && cmd[i] == '.')
			return (-1);
		i++;
	}
	if (shell->cmds->argv[1])
	{
		if (shell->cmds->argv[1][0] == '.')
			count++;
	}
	return (count);
}

char *get_executable_path(char *cmd, t_shell *shell)
{
	char *full_path;
	
	// Reset flag
	shell->flag = 0;
	
	// Handle NULL or empty command
	if (!cmd || !cmd[0])
	{
		shell->flag = 1; // Command not found
		return (NULL);
	}
	// if (ft_strchr(cmd, '/'))
	// {
	// 	if (access(cmd, X_OK) == 0)
	// 		return (ft_strdup(cmd));
	// 	else
	// 	{
	// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
	// 		ft_putstr_fd(cmd, STDERR_FILENO);
	// 		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	// 		shell->flag = 1;
	// 		return (NULL);
	// 	}
	// }
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == -1)
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": No such file or directory\n", 28);
			shell->flag = 1; // File not found
			return (NULL);
		}
		if (access(cmd, X_OK) == -1)
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Permission denied\n", 20);
			shell->flag = 4;
			return (NULL);
		}
		full_path = ft_strdup(cmd);
		if (!full_path)
		{
			write(2, "Memory allocation failed\n", 25);
			shell->flag = 1;
			return (NULL);
		}
		return (full_path);
	}
		if (cmd[0] == ':')
	{
		shell->flag = 0;
		return (NULL);
	}
	if (count_dots(cmd, shell) > 1)
	{
		ft_putstr_fd("minishell: .: .: is a directory\n", STDERR_FILENO);
		shell->flag = -1;
		return (NULL);
	}
	if (count_dots(cmd, shell) == 1)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
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

int execute_single_command(t_cmd *cmd, t_shell *shell)
{
	char *path;
	char **envp;
	int status;

	shell->flag = 0;
	path = NULL;
	
	if (cmd->argv && cmd->argv[0])
	{
		if (is_builtin(cmd->argv[0]))
			return (execute_builtin(cmd, shell));
		path = get_executable_path(cmd->argv[0], shell);
	}
	
	// Handle all error cases (errors already printed in get_executable_path)
	if (!path)
	{
		if (shell->flag == -2)
			return (2);
		if (shell->flag == -1)
			return (1);
		if (shell->flag == 0)
			return (0);
		if (shell->flag == 1)      // No such file or directory
			return (127);
		if (shell->flag == 2)      // Command not found
			return (127);
		if (shell->flag == 3)      // Is a directory
			return (126);
		if (shell->flag == 4)      // Permission denied
			return (126);
		return (127);              // Default error
	}
	
	envp = env_to_array(shell->env);
	status = fork_and_execute(path, cmd->argv, envp, shell);
	free(path);
	free_array(envp);
	return (status);
}

// Simplified fork_and_execute (errors already handled)
int fork_and_execute(char *path, char **argv, char **envp, t_shell *shell)
{
	pid_t pid;
	int status;
	void (*old_sigint)(int);

	old_sigint = signal(SIGINT, handle_sigint_single);
	pid = fork();
	if (pid == -1)
		return (signal(SIGINT, old_sigint), perror("fork"), 1);
	if (pid == 0)
	{
		if (shell->cmds->redir && setup_redirections(shell->cmds->redir) == -1)
			exit(1);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		
		// execve should not fail if get_executable_path did its job
		if (execve(path, argv, envp) == -1)
		{
			perror(argv[0]); // Fallback error message
			exit(127);
		}
	}
	shell->single_pid = pid;
	waitpid(pid, &status, 0);
	shell->single_pid = 0;
	signal(SIGINT, old_sigint);
	return (handle_single_child_signals(status));
}