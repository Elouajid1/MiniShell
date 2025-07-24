/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 10:26:20 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/18 10:27:41 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char **argv, t_shell *shell)
{
	if (argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required", STDERR_FILENO);
			write(1, "\n", 1);
			cleanup_shell(shell);
			free_tokens(&(shell->token));
			shell->last_exit_code = 2;
		}
	}
	if (argv[1] && is_numeric(argv[1]))
		shell->last_exit_code = ft_atoi(argv[1]);
	if (shell->interactive)
		ft_putendl_fd("exit", STDERR_FILENO);
	cleanup_shell(shell);
	free_tokens(&(shell->token));
	exit(shell->last_exit_code);
}


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

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	backup_fds[2];

	if (cmd->redir)
	{
		if (backup_stdio(backup_fds) == -1)
			return (1);
		if (setup_redirections(cmd->redir, shell) == -1)
		{
			restore_stdio(backup_fds);
			return (1);
		}
	}
	status = handle_builtin_logic(cmd, shell);
	if (cmd->redir)
		restore_stdio(backup_fds);
	return (status);
}


t_env	*add_env_as_node(char **env)
{
	t_env	*new_node;

	if (!env || !env[0])
		return (NULL);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(env[0]);
	if (1 == 1)
	{
		if (ft_strcmp(new_node->key, "SHLVL") == 0)
			new_node->value = ft_itoa(2);
		else
			new_node->value = ft_strdup(env[1]);
	}
	new_node->next = NULL;
	return (new_node);
}

char	**split_env_line(char *line)
{
	char	**res;
	int		pos;

	if (!line)
		return (NULL);
	res = malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	pos = 0;
	while (line[pos] && line[pos] != '=')
		pos++;
	res[0] = ft_substr(line, 0, pos);
	if (line[pos] == '=')
		res[1] = ft_strdup(line + pos + 1);
	else
		res[1] = NULL;
	res[2] = NULL;
	return (res);
}

t_env	*array_to_env(char **env, t_shell *shell)
{
	char	**splited;
	t_env	*new_node;
	t_env	*head;
	t_env	*tail;

	head = NULL;
	tail = NULL;
	shell->i = -1;
	while (env[++shell->i])
	{
		splited = split_env_line(env[shell->i]);
		if (check_for_empty(splited))
			continue ;
		new_node = add_env_as_node(splited);
		free_array(splited);
		if (!new_node)
			break ;
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
	}
	return (head);
}

int	fork_and_execute(char *path, char **argv, char **envp, t_shell *shell)
{
	pid_t	pid;
	int		status;
	void	(*old_sigint)(int);

	old_sigint = signal(SIGINT, handle_sigint_single);
	pid = fork();
	if (pid == -1)
		return (signal(SIGINT, old_sigint), perror("fork"), 1);
	if (pid == 0)
	{
		if (shell->cmds->redir
			&& setup_redirections(shell->cmds->redir, shell) == -1)
			exit(1);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(path, argv, envp) == -1)
			exit(127);
	}
	shell->single_pid = pid;
	waitpid(pid, &status, 0);
	shell->single_pid = 0;
	signal(SIGINT, old_sigint);
	return (handle_single_child_signals(status));
}

int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;
	int		status;

	shell->flag = 0;
	path = NULL;
	if (cmd->argv && cmd->argv[0])
	{
		if (is_builtin(cmd->argv[0]))
			return (execute_builtin(cmd, shell));
		path = get_executable_path(cmd->argv[0], shell);
	}
	if (!path && shell->flag == 2)
		return (handle_command_error(cmd->argv[0]));
	if (!path && shell->flag == 1)
		return (handle_path_error(cmd->argv[0]));
	envp = env_to_array(shell->env);
	status = fork_and_execute(path, cmd->argv, envp, shell);
	free(path);
	free_array(envp);
	return (status);
}

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
	shell->last_exit_code = execute_commands(shell);
	g_last_exit_code = shell->last_exit_code;
	free(command);
	free_tokens(&(shell->token));
	free_command(&(shell->cmds));
	cleanup_shell(shell);
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
	printf("exit\n");
	cleanup_shell(shell);
	return (shell->last_exit_code);
}