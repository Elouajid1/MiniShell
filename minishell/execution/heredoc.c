/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:37:29 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 12:35:22 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (str[i - 1] == '"' && str[0] == '"')
		return (1);
	if (str[i - 1] == 39 && str[0] == 39)
		return (1);
	return (0);
}

char	*evaluate_heredoc_line(char *line, t_shell *shell, char *delimiter)
{
	if (has_quotes(delimiter))
		return (ft_strdup(line));
	else if (!has_quotes(delimiter))
		return (get_heredoc_expansion(line, shell->env));
	return (NULL);
}

int	heredoc_loop(char *line, char *delimiter, t_shell *shell, int fd)
{
	shell->deli = rm_quotes(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (shell->line_count == 0)
				return (print_eof_error());
			break ;
		}
		if (ft_strcmp(shell->deli, line) == 0)
		{
			free(line);
			break ;
		}
		shell->expanded_line = evaluate_heredoc_line(line, shell, delimiter);
		if (shell->expanded_line)
			ft_putendl_fd(shell->expanded_line, fd);
		free(line);
		if (shell->expanded_line != line)
			free(shell->expanded_line);
		shell->line_count++;
	}
	free(shell->deli);
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell, int *heredoc_fd)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		handle_heredoc_child(delimiter, shell);
	waitpid(pid, &status, 0);
	return (handle_heredoc_parent(status, heredoc_fd, shell));
}

int	process_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*current;
	t_redir	*redir;
	int		fd;

	count_heredoc(cmds->redir, shell);
	current = cmds;
	while (current)
	{
		redir = current->redir;
		while (redir)
		{
			if (redir->type == R_HEREDOC)
			{
				if (shell->interepted)
					return (130);
				if (handle_heredoc(redir->file, shell, &fd) == 130)
					return (130);
				redir->heredoc_fd = fd;
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}
