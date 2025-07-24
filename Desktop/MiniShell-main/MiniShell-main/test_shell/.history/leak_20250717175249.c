

int	setup_redirections(t_redir *redir, t_shell *shell)
{
	t_redir	*current;

	if (!redir)
		return (-1);
	current = redir;
	while (current)
	{
		if (current->type == R_INPUT)
			handle_input_redirection(current->file);
		else if (current->type == R_HEREDOC)
			handle_heredoc(current->file, shell);
		else if (current->type == R_OUTPUT)
			handle_output_redirection(current->file, false);
		else if (current->type == R_APPEND)
			handle_output_redirection(current->file, true);
		current = current->next;
	}
	return (1);
}

void	begin_child_execution(t_cmd *current, int prev_fd, int *pipe_fds,
		t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_process(prev_fd, pipe_fds);
	setup_redirections(current->redir, shell);
	execute_child_command(current, shell);
}

void	execute_child_command(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;

	shell->flag = 0;
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, shell));
	path = get_executable_path(cmd->argv[0], shell);
	if (!path)
		exit(handle_command_error(cmd->argv[0]));
	envp = env_to_array(shell->env);
	if (!envp)
	{
		ft_putstr_fd("minishell: failed to convert environment\n",
			STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd->argv, envp) == -1)
	{
		perror("execve");
		free(path);
		free_array(envp);
		exit(EXIT_FAILURE);
	}
}

int	process_pipeline_commands(t_cmd *cmds, t_shell *shell)
{
	int		prev_fd;
	int		pipe_fds[2];
	t_cmd	*current;

	prev_fd = -1;
	current = cmds;
	while (current)
	{
		if (current->next && pipe(pipe_fds) == -1)
			return (perror("pipe"), 1);
		shell->single_pid = fork();
		if (shell->single_pid == -1)
			return (perror("fork"), 1);
		if (shell->single_pid == 0)
			begin_child_execution(current, prev_fd, pipe_fds, shell);
		else
		{
			shell->pipeline_pids[shell->pipeline_count++] = shell->single_pid;
			setup_parent_process(&prev_fd, pipe_fds, current->next);
		}
		current = current->next;
	}
	return (0);
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	shell->pipeline_count = 0;
	shell->g_kill_pipeline = 0;
	old_sigint = signal(SIGINT, handle_sigint_pipeline);
	old_sigquit = signal(SIGQUIT, handle_sigquit);
	if (process_pipeline_commands(cmds, shell) == 1)
	{
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
		return (1);
	}
	shell->wait_result = wait_for_children(shell);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	return (shell->wait_result);
}

int execute_commands(t_shell *shell)
{
	int exit_status;

	if (!shell || !shell->cmds)
		return (EXIT_FAILURE);

	if (!shell->cmds->next)
	{
		if (shell->cmds->argv && shell->cmds->argv[0] && is_builtin(shell->cmds->argv[0]))
			exit_status = execute_builtin(shell->cmds, shell);
		else
			exit_status = execute_single_command(shell->cmds, shell);
	}
	else
		exit_status = execute_pipeline(shell->cmds, shell);

	shell->last_exit_code = exit_status;
	return (exit_status);
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