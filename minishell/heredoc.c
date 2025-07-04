/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:37:29 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/27 17:37:33 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(char *delimiter)
{
	int pipe_fds[2];
	char *line;
	pid_t pid;
	int status;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_fds[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			ft_putendl_fd(line, pipe_fds[1]);
			free(line);
		}
		close(pipe_fds[1]);
		exit(0);
	}
	else
	{
		close(pipe_fds[1]);
		waitpid(pid, &status, 0);
		return (pipe_fds[0]);
	}
}