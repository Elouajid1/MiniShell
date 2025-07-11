/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:48:37 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/15 10:48:41 by moel-aid         ###   ########.fr       */
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
	if (!n && !argv[1])
		ft_putchar_fd('\n', STDOUT_FILENO);
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

int	builtin_pwd(void)
{
	char *cwd;
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