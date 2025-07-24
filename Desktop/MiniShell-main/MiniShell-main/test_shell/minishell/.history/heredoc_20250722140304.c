/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:37:29 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/20 15:28:19 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	heredoc_loop(char *line, char *delimiter, t_shell *shell, int fd)
{
	char	*expanded_line;

	setup_signals();
	deli = rm_quotes(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\n");
			break ;
		}
		if (ft_strcmp(deli, line) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = evaluate_heredoc_line(line, shell, delimiter);
		if (expanded_line)
			ft_putendl_fd(expanded_line, fd);
		free(line);
		if (expanded_line != line)
			free(expanded_line);
	}
	free(deli);
}

int	handle_heredoc(char *delimiter, t_shell *shell, int *heredoc_fd)
{
	char	*line;
	char	*tmp_file;
	int		write_fd;

	line = NULL;
	tmp_file = "/tmp/minishell_heredoc";
	write_fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (write_fd == -1)
	{
		perror("open");
		return (-1);
	}
	heredoc_loop(line, delimiter, shell, write_fd);
	close(write_fd);
	*heredoc_fd = open(tmp_file, O_RDONLY);
	if (*heredoc_fd == -1)
	{
		perror("open");
		unlink(tmp_file);
		return (-1);
	}
	unlink(tmp_file);
	return (0);
}

int	process_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*current;
	t_redir	*redir;
	int		fd;

	current = cmds;
	while (current)
	{
		redir = current->redir;
		while (redir)
		{
			if (redir->type == R_HEREDOC)
			{
				if (handle_heredoc(redir->file, shell, &fd) == -1)
					return (-1);
				redir->heredoc_fd = fd;
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}
