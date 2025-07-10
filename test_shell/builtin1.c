/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:56:59 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/14 16:57:03 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern t_global	g_data;

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	*home;

	if (!argv[1] || (argv[1] && argv[1][0] == '\0'))
	{
		home = getenv("HOME");
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

int	builtin_env(t_env *env)
{
	char	*path;

	while (env)
	{
		path = getenv("PATH");
		if (!path)
		{
			ft_putendl_fd("minishell: env: No such file or directory",
				STDOUT_FILENO);
			return (1);
		}
		if (env->value)
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(env->value, STDOUT_FILENO);
		}
		env = env->next;
	}
	return (0);
}

void	print_env_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		printf("KEY: %s\n", current->key);
		if (current->value)
			printf("VALUE: %s\n", current->value);
		else
			printf("VALUE: (null)\n");
		printf("-------------------\n");
		current = current->next;
	}
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;

	if (!argv[1])
		return (print_export_list(shell->env));
	i = 1;
	while (argv[i])
	{
		if (export_variable(shell, argv[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	if (argv[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (g_data.interactive)
		ft_putendl_fd("exit", STDERR_FILENO);
	cleanup_shell(shell);
	exit(shell->last_exit_code);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;

	if (argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		unset_variable(shell, argv[i]);
		i++;
	}
	return (0);
}
