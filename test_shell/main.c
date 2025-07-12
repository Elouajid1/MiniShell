/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:07:54 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/12 14:12:47 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

t_global	g_data = {0};

// void print_env_list(t_env *head)
// {
//     t_env *current = head;

//     while (current)
//     {
//         printf("KEY: %s\n", current->key);
//         if (current->value)
//             printf("VALUE: %s\n", current->value);
//         else
//             printf("VALUE: (null)\n");
//         printf("-------------------\n");
//         current = current->next;
//     }
// }

// t_cmd	*build_test_commands(t_shell *shell, char **env)
// {
// 	t_cmd	*cmd1;
// 	// t_cmd	*cmd2;
// 	// t_cmd	*cmd3;

// 	cmd1 = malloc(sizeof(t_cmd));
// 	// ---------- Command 1: cat < infile.txt ----------
// 	cmd1->argv = malloc(sizeof(char *) * 4);
// 	cmd1->argv[0] = ft_strdup("export");
// 	cmd1->argv[1] = ft_strdup("_soso23");
// 	cmd1->argv[2] = ft_strdup("_dodo=45");
// 	cmd1->argv[3] = NULL;
// 	cmd1->next = NULL;
// 	cmd1->redir = NULL;
// 	shell->env = array_to_env(env);
// 	return (cmd1);
// }

int	main_loop(t_shell *shell, char **env)
{
	char	*command;
	char	**cmd_array;

	setup_signals();
	shell->env = array_to_env(env);
	shell->token = NULL;
	while (1)
	{
		command = readline("minishell$ ");
		if (!command)
			break ;
		if (*command == '\0')
		{
			free(command);
			// cleanup_shell(shell);
			continue ;
		}
		add_history(command);
		cmd_array = ft_split1(command, 32);
		if (!cmd_array)
		{
			free (command);
			return (0);
		}
		tokenize(&(shell->token), cmd_array);
		if (shell->token == NULL)
		{
			free (command);
			continue ;
		}
		if (syntax_errors(shell->token))
		{
			free(command);
			free_tokens(&shell->token);
			continue ;		
		}
		expand(&(shell->token), shell->env);
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
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	// char *str;
	int i;
	// int pos;
	// char *path;
	// char *command;
	t_shell *shell;

	i = 0;
	(void)ac;
	(void)av;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (EXIT_FAILURE);
	g_data.interactive = isatty(STDIN_FILENO);
	main_loop(shell, env);
	printf("exit\n");
	return (shell->last_exit_code);
}