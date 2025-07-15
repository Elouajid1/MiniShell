/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:07:54 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/14 13:35:13 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

int			g_last_exit_code = 0;

static int	handle_empty_or_tokenize(t_shell *shell, char *command)
{
	char	**cmd_array;

	if (*command == '\0')
	{
		free(command);
		return (1);
	}
	add_history(command);
	cmd_array = ft_split1(command, 32);
	if (!cmd_array)
	{
		free(command);
		return (0);
	}
	tokenize(&(shell->token), cmd_array);
	if (shell->token == NULL)
	{
		free(command);
		return (1);
	}
	return (2);
}

static int	syntax_check_and_expand(t_shell *shell, char *command)
{
	if (syntax_errors(shell->token))
	{
		free(command);
		return (1);
	}
	expand(&(shell->token), shell->env);
	return (2);
}

static void	execute_and_cleanup(t_shell *shell, char *command)
{
	shell->cmds = parse(&(shell->command), shell->token);
	shell->last_exit_code = execute_commands(shell);
	g_last_exit_code = shell->last_exit_code;
	free(command);
	free_tokens(&(shell->token));
	shell->token = NULL;
	shell->command = NULL;
}

int	main_loop(t_shell *shell, char **env)
{
	char	*command;
	int		result;

	(void)env;
	setup_signals();
<<<<<<< HEAD
	shell->interactive = isatty(STDIN_FILENO);
=======
	shell->env = array_to_env(env);
	shell->token = NULL;
>>>>>>> 0af92a97d55befeeb1d5664cc8c8adea33bd5a8b
	while (1)
	{
		command = readline("minishell$ ");
		if (!command)
			break ;
		result = handle_empty_or_tokenize(shell, command);
		if (result == 0)
			return (0);
		if (result == 1)
			continue ;
<<<<<<< HEAD
		result = syntax_check_and_expand(shell, command);
		if (result == 1)
			continue ;
		execute_and_cleanup(shell, command);
=======
		}
		if (syntax_errors(shell->token))
		{
			free(command);
			free_tokens(&shell->token);
			continue ;		
		}
		expand(&(shell->token), shell->env);
		q_rremoval(&shell->token);
		shell->cmds = parse(&(shell->command), shell->token);
		// shell->cmds = build_test_commands(shell, env);
		shell->last_exit_code = execute_commands(shell);
		g_data.last_exit_status = shell->last_exit_code;
		// printf("gogo\n");
		// free_array(shell->cmds->argv);
		free(command);
		free_tokens(&shell->token);
		free_command(&shell->cmds);
		// free_command(&(shell->command));
		shell->token = NULL;
		shell->command = NULL;
>>>>>>> 0af92a97d55befeeb1d5664cc8c8adea33bd5a8b
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
<<<<<<< HEAD
	char	*str;
	int		pos;
	char	*path;
	char	*command;
	t_shell	*shell;

=======
	// char *str;
	int i;
	// int pos;
	// char *path;
	// char *command;
	t_shell *shell;

	i = 0;
	(void)ac;
	(void)av;
>>>>>>> 0af92a97d55befeeb1d5664cc8c8adea33bd5a8b
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (EXIT_FAILURE);
	shell->shell_lvl = 1;
	if (!envp || !envp[0])
	{
		shell->env = NULL;
		shell->env_flag = 1;
		shell->env = init_minimal_env(shell);
	}
	else
	{
		shell->env_flag = 0;
		shell->env = array_to_env(envp, shell);
	}
	main_loop(shell, envp);
	printf("exit\n");
	return (shell->last_exit_code);
}
