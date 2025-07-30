/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command_signal.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:26:59 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:02:06 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

void	handle_sigint_single(int sig)
{
	sig++;
	return ;
}

void	handle_sigquit_single(int sig)
{
	sig++;
	g_last_exit_code = 131;
	return ;
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
	return (WEXITSTATUS(status));
}
