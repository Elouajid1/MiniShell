/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:36 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/13 18:38:44 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

extern int	g_last_exit_code;

int	handle_output_redirection(char *file, bool append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_input_redirection(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	setup_redirections(t_redir *redir, t_shell *shell)
{
	t_redir	*current;
	int		fd;

	if (!redir)
		return (-1);
	current = redir;
	while (current)
	{
		if (current->type == R_INPUT)
			handle_input_redirection(current->file);
		else if (current->type == R_HEREDOC)
			handle_heredoc(current->file, shell, &fd);
		else if (current->type == R_OUTPUT)
			handle_output_redirection(current->file, false);
		else if (current->type == R_APPEND)
			handle_output_redirection(current->file, true);
		current = current->next;
	}
	return (1);
}

int	handle_pipe_creation(t_cmd *current, int *pipe_fds)
{
	if (current->next && pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}
