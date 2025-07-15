/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:25:15 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/15 11:25:18 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	count_env_with_values(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		if (current->value)
			count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_string(t_env *env_node)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(env_node->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, env_node->value);
	free(temp);
	return (result);
}

void	setup_nodes()