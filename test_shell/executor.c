/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:01:19 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/11 18:06:16 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

extern t_global	g_data;

int	fork_and_execute(char *path, char **argv, char **envp, t_shell *shell)
{
	pid_t	pid;
	int		status;
	// char	*tmp;
	void	(*old_sigint)(int);

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
			exit(127);
	}
	g_data.single_pid = pid;
	waitpid(pid, &status, 0);
	g_data.single_pid = 0;
	signal(SIGINT, old_sigint);
	return (handle_single_child_signals(status));
}

int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;
	int		status;
	
	g_data.flag = 0;
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0]))
		return (execute_builtin(cmd, shell));
	path = get_executable_path(cmd->argv[0]);
	if (!path && g_data.flag == 2)
		return (handle_command_error(cmd->argv[0]));
	if (!path && g_data.flag == 1)
		return (handle_path_error(cmd->argv[0]));
	envp = env_to_array(shell->env);
	status = fork_and_execute(path, cmd->argv, envp, shell);
	free(path);
	free_array(envp);
	return (status);
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	g_data.pipeline_count = 0;
	g_data.g_kill_pipeline = 0;
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

int	execute_commands(t_shell *shell)
{
	int	exit_status;

	if (!shell || !shell->cmds)
		return (EXIT_FAILURE);
	if (!shell->cmds->next)
	{
		if (is_builtin(shell->cmds->argv[0]) && !shell->cmds->redir)
			exit_status = execute_builtin(shell->cmds, shell);
		else
			exit_status = execute_single_command(shell->cmds, shell);
	}
	else
		exit_status = execute_pipeline(shell->cmds, shell);
	shell->last_exit_code = exit_status;
	return (exit_status);
}
