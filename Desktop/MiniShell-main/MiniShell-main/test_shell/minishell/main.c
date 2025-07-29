/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:07:54 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/29 03:49:39 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

int			g_last_exit_code = 0;

int	check_sp(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == 32)
			return (1);
		i++;
	}
	return (0);
}

int	count_fields(char **strs, t_env *env)
{
	char	*expanded;
	char	**fields;
	int		count;
	int		i;

	i = 0;
	count = 0;
	if (!strs)
		return (0);
	while (strs[i])
	{
		if (ft_strcmp("<<", strs[i]) == 0)
		{
			count++;
			i++;
		}
		else
		{
			fields = NULL;
			expanded = expand_str(strs[i], env);
			if (check_sp(expanded))
			{
				fields = ft_split1(expanded, 32);
				count += argv_len(fields);
			}
			else
				count++;
			free(expanded);
			free_array(fields);
			i++;
		}
	}
	return (count);
}

char	**expander(char **strs, t_env *env)
{
	char	**res;
	char	*expanded;
	char	**tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * (count_fields(strs, env) + 1));
	if (!res)
		return (NULL);
	while (strs[i])
	{
		if (ft_strcmp("<<", strs[i]) == 0)
		{
			res[j++] = ft_strdup(strs[i++]);
			if (strs[i])
				res[j++] = ft_strdup(strs[i++]);
			continue ;
		}
		else
		{
			expanded = expand_str(strs[i], env);
			if (check_sp(expanded))
			{
				tmp = ft_split1(expanded, 32);
				char **iter = tmp;
				while (*iter)
				{
					res[j] = ft_strdup(*iter);
					j++;
					iter++;
				}
				free_array(tmp);
			}
			else
			{
				res[j] = ft_strdup(expanded);
				j++;
			}
			free(expanded);
			i++;
		}
	}
	res[j] = NULL;
	return (res);
}

static int	handle_empty_or_tokenize(t_shell *shell, char *command)
{
	char	**cmd_array;
	char	**splitted;

	shell->token = NULL;
	if (*command == '\0')
	{
		free(command);
		return (1);
	}
	add_history(command);
	splitted = ft_split1(command, 32);
	cmd_array = expander(splitted, shell->env);
	if (!cmd_array)
	{
		free(command);
		return (0);
	}
	tokenize(&(shell->token), cmd_array);
	if (shell->token == NULL)
	{
		free_array(cmd_array);
		free(command);
		return (1);
	}
	free_array(splitted);
	free_array(cmd_array);
	return (2);
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
	// expand(&(shell->token), shell->env);
	q_rremoval(&(shell->token), shell);
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
		command = NULL;
		shell->token = NULL;
		shell->cmds = NULL;
		command = readline("minishell$ ");
		if (!command)
		{
			shell->last_exit_code = 0;
			break ;
		}
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
	shell->shell_lvl = 1;
	if (!envp || !envp[0])
	{
		shell->env = NULL;
		shell->env_flag = 1;
		shell->env = init_minimal_env(shell);
		shell->last_exit_code = 0;
	}
	shell->env_flag = 0;
	shell->env = array_to_env(envp, shell);
	main_loop(shell, envp);
	cleanup_shell(shell, true);
	printf("exit\n");
	status = shell->last_exit_code;
	free(shell);
	return (status);
}
