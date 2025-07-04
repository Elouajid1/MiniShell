/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command_signal.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:26:59 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/30 12:27:03 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_global	g_data;

void	handle_sigint_single(int sig)
{
	if (g_data.single_pid > 0)
		kill(g_data.single_pid, SIGINT);
}

void	handle_sigquit_single(int sig)
{
	if (g_data.single_pid > 0)
		kill(g_data.single_pid, SIGQUIT);
}

int	handle_single_child_signals(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			return (write(STDOUT_FILENO, "\n", 1), 130);
		if (sig == SIGQUIT)
			return (write(STDOUT_FILENO, "Quit (core dumped)\n", 19), 131);
		return (128 + sig);
	}
	// if (WIFEXITED(status))
	// 	return (WIFEXITED(status));
	return (0);
}
