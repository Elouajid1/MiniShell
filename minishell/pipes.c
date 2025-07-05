/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:50:06 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/15 10:50:10 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern t_global	g_data;

void	setup_child_process(t_cmd *cmd, int prev_fd, int *pipe_fds)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (pipe_fds)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

void	setup_parent_process(int *prev_fd, int *pipe_fds, bool has_next)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		close(pipe_fds[1]);
		*prev_fd = pipe_fds[0];
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
		g_data.single_pid = fork();
		if (g_data.single_pid == -1)
			return (perror("fork"), 1);
		if (g_data.single_pid == 0)
			begin_child_execution(current, prev_fd, pipe_fds, shell);
		else
		{
			g_data.pipeline_pids[g_data.pipeline_count++] = g_data.single_pid;
			setup_parent_process(&prev_fd, pipe_fds, current->next);
		}
		current = current->next;
	}
	return (0);
}

void	execute_child_command(t_cmd *cmd, t_shell *shell)
{
	char *path;
	char **envp;

	path = get_executable_path(cmd->argv[0]);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, shell));
	if (!path)
        exit(handle_command_error(cmd->argv[0]));
	envp = env_to_array(shell->env);
	if (!envp)
	{
		ft_putstr_fd("minishell: failed to convert environment\n",
			STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (absolute_path(cmd->argv[0]))
		path = argv[0];
	if (execve(path, cmd->argv, envp) == -1)
	{
		perror("execve");
		free(path);
		free_array(envp);
		exit(EXIT_FAILURE);
	}
}