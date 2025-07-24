/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:13:21 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 10:27:00 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_last_exit_code;

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_prompt;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = ha;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_sigint_pipeline(int sig)
{
	sig++;
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_sigint_prompt(int sig)
{
	sig++;
	write(STDOUT_FILENO, "\n", 1);
	g_last_exit_code = 130;
	if (isatty(STDIN_FILENO))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigquit(int sig)
{
	g_last_exit_code = 131;
	sig++;
}
