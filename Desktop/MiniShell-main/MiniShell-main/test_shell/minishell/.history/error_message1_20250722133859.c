/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_error_codes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:20:19 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 17:20:23 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	return_error_code(t_shell *shell)
{
	if (shell->flag == -2)
		return (2);
	if (shell->flag == -1)
		return (1);
	if (shell->flag == 0)
		return (0);
	if (shell->flag == 1)
		return (127);
	if (shell->flag == 2)
		return (127);
	if (shell->flag == 3)
		return (1);
	if (shell->flag == 4)
		return (126);
	return (127);
}

int	exit_error_code(char *argv)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(argv, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required", STDERR_FILENO);
	write(1, "\n", 1);
	return (2);
}

int	print_pwd_error(char *option)
{
	ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
	ft_putstr_fd(option, STDERR_FILENO);
	ft_putendl_fd(": innvalid option", STDERR_FILENO);
	ft_putendl_fd("pwd: usage: pwd [-LP]", STDERR_FILENO);
	return (2);
}

int	print_cd_error(void)
{
	ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
	return (1);
}

char	*print_dot_error(t_shell *shell)
{
	ft_putstr_fd("minishell: .: .: is a directory\n", STDERR_FILENO);
	shell->flag = 3;
	return (NULL);
}