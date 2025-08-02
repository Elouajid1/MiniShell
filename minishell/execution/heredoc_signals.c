/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:31:54 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 14:50:25 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_last_exit_code = 130;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}
