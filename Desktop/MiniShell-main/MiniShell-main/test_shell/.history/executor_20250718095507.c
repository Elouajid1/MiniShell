/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:01:19 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/03 16:01:23 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern int	g_last_exit_code;

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

int	setup_redirections(t_redir *redir, t_shell *shell)
{
	t_redir	*current;
	int		fd;

	if (!redir)
		return (-1);
	current = redir;
	while (current)
	{
		if (current->type == R_INPUT)
			handle_input_redirection(current->file);
		else if (current->type == R_HEREDOC)
		{
			if (handle_heredoc(current->file, shell, &fd) == -1)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->type == R_OUTPUT)
			handle_output_redirection(current->file, false);
		else if (current->type == R_APPEND)
			handle_output_redirection(current->file, true);
		current = current->next;
	}
	return (1);
}
