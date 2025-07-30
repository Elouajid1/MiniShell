/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:25:15 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:01:28 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	check_for_empty(char **splited)
{
	if (!splited || !(*splited))
		return (1);
	return (0);
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
