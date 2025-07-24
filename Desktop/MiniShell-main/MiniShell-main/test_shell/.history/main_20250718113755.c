/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:07:54 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/10 18:49:35 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

int			g_last_exit_code = 0;

static int	handle_empty_or_tokenize(t_shell *shell, char *command)
{
	char	**cmd_array;

	shell->token = NULL;
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
	q_rremoval(&(shell->token));
	return (2);
}

static void	execute_and_cleanup(t_shell *shell, char *command)
{
	shell->cmds = parse(&(shell->cmds), shell->token);
	int i = 0;
	while (shell->cmds->argv[i])
		printf("%s\n", shell->cmds->argv[i])
	shell->last_exit_code = execute_commands(shell);
	g_last_exit_code = shell->last_exit_code;
	// free(command);
	// free_tokens(&(shell->token));
	// free_command(&(shell->cmds));
	// cleanup_shell(shell, false);
	shell->token = NULL;
	command = NULL;
}

int	main_loop(t_shell *shell, char **env)
{
	char	*command;
	int		result;

	(void)env;
	setup_signals();
	shell->interactive = isatty(STDIN_FILENO);
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
		result = syntax_check_and_expand(shell, command);
		if (result == 1)
			continue ;
		execute_and_cleanup(shell, command);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	ac++;
	av++;
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
	cleanup_shell(shell, true);
	printf("exit\n");
	return (shell->last_exit_code);
}
