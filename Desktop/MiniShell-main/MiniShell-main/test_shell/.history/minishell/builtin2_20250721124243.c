/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:48:37 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:25:29 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strcmp("env", cmd) == 0 || ft_strcmp("echo", cmd) == 0
		|| ft_strcmp("cd", cmd) == 0 || ft_strcmp("pwd", cmd) == 0
		|| ft_strcmp("unset", cmd) == 0 || ft_strcmp("exit", cmd) == 0
		|| ft_strcmp("export", cmd) == 0);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n;

	n = 1;
	i = 1;
	if (argv[1] && (ft_strcmp(argv[1], "-n") == 0))
	{
		n = 0;
		i = 2;
	}
	while (argv[i] && argv[1])
	{
		ft_putendl_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (n && !argv[1])
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	builtin_env(t_env *env, t_shell *shell)
{
	char	*path;
	t_env	*current;

	current = env;
	path = find_env_key(current, "PATH");
	if (env_error_message(path))
		return (1);
	if (!shell->env_flag)
	{
		while (current)
		{
			if (current->value)
			{
				ft_putstr_fd(current->key, STDOUT_FILENO);
				ft_putchar_fd('=', STDOUT_FILENO);
				ft_putendl_fd(current->value, STDOUT_FILENO);
			}
			current = current->next;
		}
	}
	else
		print_list(env);
	return (0);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;

	if (!argv[1])
	{
		if (shell->env_flag == 0)
			return (print_export_list(shell->env, shell));
		else
			return (print_export_list1(shell->env));
	}
	i = 1;
	while (argv[i])
	{
		if (export_variable(shell, argv[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}
