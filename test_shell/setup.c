/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:57:47 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/15 11:57:50 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
	int		len;

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
