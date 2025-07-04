/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:25:16 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/11 18:25:32 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern t_global g_data;

int	wait_for_children(t_shell *shell)
{
	shell->i = -1;
	shell->status = 0;
	shell->printed = false;
	while (++shell->i < g_data.pipeline_count)
	{
		waitpid(g_data.pipeline_pids[shell->i], &shell->status, 0);
		if (WIFSIGNALED(shell->status))
		{
			shell->sig = WTERMSIG(shell->status);
			if (shell->sig == SIGINT)
				shell->last_status = 130;
			else if (shell->sig == SIGQUIT && !shell->printed)
			{
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
				shell->printed = true;
				shell->last_status = 131;
			}
			else
				shell->last_status = 128 + shell->sig;
		}
		else if (WIFEXITED(shell->status))
			shell->last_status = WEXITSTATUS(shell->status);
	}
	return (shell->last_status);
}

void	close_all_pipes(int pipes[][2], int pipe_count)
{
	int	i;

	i = -1;
	while (++i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
