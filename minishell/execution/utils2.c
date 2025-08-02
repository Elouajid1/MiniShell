/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 18:23:43 by moel-aid          #+#    #+#             */
/*   Updated: 2025/08/01 18:23:46 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_child(char *delimiter, t_shell *shell)
{
	int		write_fd;
	char	*line;
	char	*tmp_file;

	line = NULL;
	tmp_file = "/tmp/minishell_heredoc";
	signal(SIGINT, heredoc_sigint_handler);
	write_fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (write_fd == -1)
	{
		perror("open");
		exit(1);
	}
	heredoc_loop(line, delimiter, shell, write_fd);
	close(write_fd);
	exit(0);
}

int	handle_heredoc_parent(int status, int *heredoc_fd, t_shell *shell)
{
	char	*tmp_file;

	tmp_file = "/tmp/minishell_heredoc";
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		shell->last_exit_code = 130;
		unlink(tmp_file);
		return (130);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		shell->last_exit_code = 130;
		unlink(tmp_file);
		return (130);
	}
	*heredoc_fd = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	if (*heredoc_fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (0);
}
