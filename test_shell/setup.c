/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:57:47 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/11 17:41:50 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_process(t_cmd *cmd, int prev_fd, int *pipe_fds)
{
	(void)cmd;
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (pipe_fds)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

void	setup_parent_process(int *prev_fd, int *pipe_fds, bool has_next)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		close(pipe_fds[1]);
		*prev_fd = pipe_fds[0];
	}
}

t_env	*add_env_as_node(char **env)
{
	t_env	*new_node;

	if (!env || !env[0])
		return (NULL);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(env[0]);
	new_node->value = ft_strdup(env[1]);
	new_node->next = NULL;
	return (new_node);
}

char	**split_env_line(char *line)
{
	char	**res;
	int		pos;
	// int		len;

	if (!line)
		return (NULL);
	res = malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	pos = 0;
	while (line[pos] && line[pos] != '=')
		pos++;
	res[0] = ft_substr(line, 0, pos);
	if (line[pos] == '=')
		res[1] = ft_strdup(line + pos + 1);
	else
		res[1] = NULL;
	res[2] = NULL;
	return (res);
}

t_env	*array_to_env(char **env)
{
	int		i;
	char	**splited;
	t_env	*new_node;
	t_env	*head;
	t_env	*tail;

	i = -1;
	head = NULL;
	tail = NULL;
	while (env[++i])
	{
		splited = split_env_line(env[i]);
		if (!splited || !splited[0])
		{
			free_array(splited);
			continue ;
		}
		new_node = add_env_as_node(splited);
		if (!new_node)
			break ;
		free_array(splited);
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
	}
	return (head);
}
