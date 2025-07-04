/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:13:21 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/29 13:13:23 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern t_global	g_data;

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_prompt;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
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
	int	i;

	i = 0;
	g_data.g_kill_pipeline = 1;
	write(STDOUT_FILENO, "\n", 1);
	while (i < g_data.pipeline_count)
	{
		kill(g_data.pipeline_pids[i], SIGINT);
		i++;
	}
}

void	handle_sigint_prompt(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	if (isatty(STDIN_FILENO))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigquit(int sig)
{
	int i;

	i = 0;
	while (i < g_data.pipeline_count)
	{
		kill(g_data.pipeline_pids[i], SIGINT);
		i++;
	}
}