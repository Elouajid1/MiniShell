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
	char	*deli;

	setup_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\n");
			break ;
		}
		deli = rm_quotes(delimiter);
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
}

int	handle_heredoc(char *delimiter, t_shell *shell, int *heredoc_fd)
{
	char	*line;
	char	*tmp_file;
	int		write_fd;

	line = NULL;
	tmp_file = "/tmp/minishell_heredoc";
	
	// Open for writing
	write_fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (write_fd == -1)
	{
		perror("open");
		return (-1);
	}
	
	// Write heredoc content
	heredoc_loop(line, delimiter, shell, write_fd);
	close(write_fd);
	
	// Open for reading and return the fd
	*heredoc_fd = open(tmp_file, O_RDONLY);
	if (*heredoc_fd == -1)
	{
		perror("open");
		unlink(tmp_file);
		return (-1);
	}
	
	unlink(tmp_file);  // Clean up temp file
	return (0);  // Success
}
