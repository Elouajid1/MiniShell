/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:02:05 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/30 14:02:10 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_command_error(char *cmd_name)
{
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putendl_fd(": commannnd not found", STDERR_FILENO);
	return (127);
}

int	handle_path_error(char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (127);
}

int	env_error_message(char *path)
{
	if (!path)
	{
		ft_putendl_fd("minishell: env: No such file or directory",
			STDOUT_FILENO);
		return (1);
	}
	return (0);
}
