/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:07:54 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:09:08 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			g_last_exit_code = 0;

static int	handle_empty_or_tokenize(t_shell *shell, char *command)
{
	char	**cmd_array;
	char	**splitted;

	if (*command == '\0')
		return (free(command), 1);
	add_history(command);
	splitted = ft_split1(command, 32);
	cmd_array = expander(splitted, shell->env);
	if (!cmd_array)
	{
		free(command);
		free(shell->args);
		free_array(splitted);
		return (0);
	}
	tokenize(&(shell->token), cmd_array);
	if (shell->token == NULL)
	{
		free_array(cmd_array);
		free_array(splitted);
		free(command);
		return (1);
	}
	return (free_array(splitted), free_array(cmd_array), 2);
}

static int	syntax_check_and_expand(t_shell *shell, char *command)
{
	if (syntax_errors(shell->token))
	{
		free(command);
		free_tokens(&(shell->token));
		free_cmd_list(shell->cmds);
		return (1);
	}
	if (q_rremoval(&(shell->token)))
	{
		free(command);
		free_tokens(&(shell->token));
		free_cmd_list(shell->cmds);
		return (1);
	}
	return (2);
}

static void	execute_and_cleanup(t_shell *shell, char *command)
{
	shell->cmds = parse(&(shell->cmds), shell->token);
	shell->last_exit_code = execute_commands(shell);
	g_last_exit_code = shell->last_exit_code;
	free(command);
	cleanup_shell(shell, false);
	shell->token = NULL;
	shell->cmds = NULL;
	shell->args = NULL;
}

int	main_loop(t_shell *shell, char **env)
{
	char	*command;
	int		result;

	(void)env;
	setup_signals();
	while (1)
	{
		command = NULL;
		shell->token = NULL;
		shell->cmds = NULL;
		shell->args = NULL;
		command = readline("minishell$ ");
		if (!command)
			return (shell->last_exit_code = 0, 0);
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
	int		status;

	ac++;
	av++;
	status = 0;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (EXIT_FAILURE);
	if (!envp || !envp[0])
	{
		shell->env = NULL;
		shell->env_flag = 1;
		shell->env = init_minimal_env(shell);
		shell->last_exit_code = 0;
	}
	shell->env = array_to_env(envp, shell);
	shell->interactive = isatty(STDIN_FILENO);
	main_loop(shell, envp);
	cleanup_shell(shell, true);
	printf("exit\n");
	status = shell->last_exit_code;
	free(shell);
	return (status);
}
