/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:01:19 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 13:10:41 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

int	fork_and_execute(char *path, char **argv, char **envp, t_shell *shell)
{
	pid_t	pid;
	void	(*old_sigint)(int);

	shell->exit_status = 0;
	old_sigint = signal(SIGINT, handle_sigint_single);
	pid = fork();
	if (pid == -1)
		return (signal(SIGINT, old_sigint), perror("fork"), 1);
	if (pid == 0)
	{
		if (shell->cmds->redir && setup_redirections(shell->cmds->redir) == -1)
			exit(1);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(path, argv, envp) == -1)
		{
			perror(argv[0]);
			exit(127);
		}
	}
	shell->single_pid = pid;
	waitpid(pid, &shell->exit_status, 0);
	shell->single_pid = 0;
	signal(SIGINT, old_sigint);
	return (handle_single_child_signals(shell->exit_status));
}

int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;
	int		status;

	shell->flag = 0;
	path = NULL;
	status = 0;
	if (cmd->argv && cmd->argv[0])
	{
		if (is_builtin(cmd->argv[0]))
			return (execute_builtin(cmd, shell));
		path = get_executable_path(cmd->argv[0], shell);
	}
	if (!path)
		return (return_error_code(shell));
	envp = env_to_array(shell->env);
	status = fork_and_execute(path, cmd->argv, envp, shell);
	free(path);
	free_array(envp);
	return (status);
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
	old_sigint = signal(SIGINT, handle_sigint_pipeline);
	old_sigquit = signal(SIGQUIT, sigquit_handler);
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

int	execute_commands(t_shell *shell)
{
	int	exit_status;
	int	code;

	code = process_heredocs(shell->cmds, shell);
	exit_status = 0;
	if (!shell || !shell->cmds)
		return (EXIT_FAILURE);
	if (code == -1)
		return (EXIT_FAILURE);
	else if (code == 130)
		return (130);
	if (!shell->cmds->next)
	{
		if (shell->cmds->argv && shell->cmds->argv[0]
			&& is_builtin(shell->cmds->argv[0]))
			exit_status = execute_builtin(shell->cmds, shell);
		else
			exit_status = execute_single_command(shell->cmds, shell);
	}
	else
		exit_status = execute_pipeline(shell->cmds, shell);
	shell->last_exit_code = exit_status;
	return (exit_status);
}
