/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:35:17 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:01:15 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*print_single_dot_error(t_shell *shell)
{
	ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
	ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
	shell->flag = -2;
	return (NULL);
}

void	print_heredoc_error(t_shell *shell)
{
	ft_putstr_fd("minishell: : maximum here-document count exceeded", 2);
	cleanup_shell(shell, true);
	free(shell);
	exit(2);
}

void	count_heredoc(t_redir *redir, t_shell *shell)
{
	t_redir	*current;
	int		count;

	count = 0;
	current = redir;
	while (current)
	{
		if (current->type == R_HEREDOC)
		{
			if (count > 16)
				print_heredoc_error(shell);
			count++;
		}
		current = current->next;
	}
}

int	print_eof_error(void)
{
	ft_putstr_fd("minishell: warning: here-document at line 2 ", 2);
	ft_putendl_fd("delimited by end-of-file (wanted `end')", 2);
	return (0);
}

int	print_exit_error(char *cmd)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": numeric argument required", 2);
	return (2);
}
