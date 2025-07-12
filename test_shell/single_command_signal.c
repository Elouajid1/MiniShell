/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command_signal.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:26:59 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/11 17:44:13 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_global	g_data;

void	handle_sigint_single(int sig)
{
	(void)sig;
	if (g_data.single_pid > 0)
		kill(g_data.single_pid, SIGINT);
}

void	handle_sigquit_single(int sig)
{
	(void)sig;
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
	return (0);
}
