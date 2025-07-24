/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:16:52 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/21 11:14:07 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static void	parse_key_value(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

static void	update_existing_env(t_env *existing, char *key, char *value)
{
	if (value)
	{
		free(existing->value);
		existing->value = value;
	}
	free(key);
}

static int	create_new_env(t_shell *shell, char *key, char *value)
{
	t_env	*new_node;

	new_node = create_env_node(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		return (1);
	}
	add_env_node(&shell->env, new_node);
	free(key);
	free(value);
	return (0);
}

int	export_variable(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	t_env	*existing;

	if (!is_valid_identifier(arg))
	{
		print_export_error(arg);
		return (1);
	}
	parse_key_value(arg, &key, &value);
	existing = find_env_node(shell->env, key);
	if (existing)
	{
		update_existing_env(existing, key, value);
		if (value != existing->value)
			free(value);
	}
	else
		return (create_new_env(shell, key, value));
	return (0);
}
