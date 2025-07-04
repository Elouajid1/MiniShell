/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:01:42 by moel-aid          #+#    #+#             */
/*   Updated: 2025/05/28 17:28:09 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	begin_child_execution(t_cmd *current, int prev_fd, int *pipe_fds,
		t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_process(current, prev_fd, pipe_fds);
	setup_redirections(current->redir);
	execute_child_command(current, shell);
}

bool	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

char	*get_executable_path(char *cmd)
{
	char	*full_path;
	char	**split;
	char	*path_part;
	char	**s_cmd;
	int		i;
	int		value;

	i = -1;
	full_path = getenv("PATH");
	split = ft_split(full_path, ':');
	s_cmd = ft_split(cmd, ' ');
	while (split && split[++i])
	{
		path_part = ft_strjoin(split[i], "/");
		full_path = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		value = access(full_path, F_OK | X_OK);
		if (value == 0)
		{
			free_array(split);
			free_array(s_cmd);
			return (full_path);
		}
		else if (value == -1)
			return (NULL);
		free(full_path);
	}
	free_array(split);
	return (cmd);
}
