/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:50:06 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:26:50 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_last_exit_code;

int	backup_stdio(int backup[2])
{
	backup[0] = dup(STDIN_FILENO);
	backup[1] = dup(STDOUT_FILENO);
	if (backup[0] == -1 || backup[1] == -1)
		return (-1);
	return (0);
}

void	restore_stdio(int backup[2])
{
	dup2(backup[0], STDIN_FILENO);
	dup2(backup[1], STDOUT_FILENO);
	close(backup[0]);
	close(backup[1]);
}

void	begin_child_execution(t_cmd *current, int prev_fd, int *pipe_fds,
		t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_process(prev_fd, pipe_fds);
	setup_redirections(current->redir);
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
	if (!path && shell->flag == 2)
		exit(handle_command_error(cmd->argv[0]));
	if (!path && shell->flag == 1)
		exit(handle_path_error(cmd->argv[0]));
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
