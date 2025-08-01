/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:02:05 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 14:45:57 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_command_error(char *cmd_name)
{
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	return (127);
}

int	handle_path_error(char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (127);
}

int	env_error_message(char *path, t_shell *shell)
{
	if (!path)
	{
		ft_putendl_fd("minishell: env: No such file or directory",
			STDOUT_FILENO);
		return (127);
	}
	else if (path && shell->cmds->argv[1])
	{
		ft_putstr_fd("env: ", 2);
		write(1, "'", 1);
		ft_putstr_fd(shell->cmds->argv[1], 2);
		write(1, "'", 1);
		ft_putendl_fd(": No such file or directory",
			STDOUT_FILENO);
		return (127);
	}
	return (0);
}

char	*no_such_file_error(char *cmd, t_shell *shell)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": No such file or directory\n", 28);
	shell->flag = 1;
	return (NULL);
}

char	*permission_error(char *cmd, t_shell *shell)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Permission denied\n", 20);
	shell->flag = 4;
	return (NULL);
}
