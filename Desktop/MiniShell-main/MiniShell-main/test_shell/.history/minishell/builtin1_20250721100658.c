/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:56:59 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:25:23 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin_logic(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp("env", cmd->argv[0]) == 0)
		return (builtin_env(shell->env, shell));
	if (ft_strcmp("echo", cmd->argv[0]) == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strcmp("cd", cmd->argv[0]) == 0)
		return (builtin_cd(cmd->argv, shell));
	if (ft_strcmp("pwd", cmd->argv[0]) == 0)
		return (builtin_pwd());
	if (ft_strcmp("unset", cmd->argv[0]) == 0)
		return (builtin_unset(cmd->argv, shell));
	if (ft_strcmp("export", cmd->argv[0]) == 0)
		return (builtin_export(cmd->argv, shell));
	if (ft_strcmp("exit", cmd->argv[0]) == 0)
		return (builtin_exit(cmd->argv, shell));
	return (1);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	*home;

	if (!argv[1] || (argv[1] && argv[1][0] == '\0'))
	{
		home = find_env_key(shell->env, "HOME");
		if (!home)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDOUT_FILENO);
			return (1);
		}
		path = home;
	}
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDOUT_FILENO);
		perror(path);
		return (1);
	}
	return (0);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int status;

	status = 0;
	if (argv[0] && argv[2] && argv[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
			status = exit_error_code(argv[1]);
	}
	if (argv[1] && is_numeric(argv[1]))
		status = ft_atoi(argv[1]);
	if (shell->interactive)
		ft_putendl_fd("exit", STDERR_FILENO);
	cleanup_shell(shell, true);
	free_tokens(&(shell->token));
	free(shell);
	exit(status);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		unset_variable(shell, argv[i]);
		i++;
	}
	return (0);
}
