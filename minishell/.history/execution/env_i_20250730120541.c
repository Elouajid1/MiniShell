/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_i.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 13:15:06 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:00:56 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_values(t_env *current)
{
	if (current->show_in_export)
	{
		ft_putstr_fd(current->key, 1);
		if (current->value)
		{
			ft_putchar_fd('=', 1);
			ft_putchar_fd(34, 1);
		}
		ft_putstr_fd(current->value, 1);
		if (!current->value)
			write(1, "\n", 1);
		if (current->value)
		{
			ft_putchar_fd(34, 1);
			write(1, "\n", 1);
		}
	}
}

t_env	*new_env(const char *key, const char *value, int show_env,
		int show_export)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (key)
		node->key = strdup(key);
	else
		node->key = NULL;
	if (value)
		node->value = strdup(value);
	else
		node->value = NULL;
	node->show_in_env = show_env;
	node->show_in_export = show_export;
	node->next = NULL;
	return (node);
}

static void	link_env_nodes(t_env *pwd, t_env *shlvl, t_env *oldpwd,
		t_env *underscore)
{
	t_env	*path;
	char	*joined;

	path = new_env("PATH", "/home/moel-aid/bin:/usr/local/sbin:/usr/local/bin:",
			0, 0);
	joined = ft_strjoin(path->value, "/usr/sbin:/usr/bin:/sbin:/bin:");
	free(path->value);
	path->value = ft_strjoin(joined, "/usr/games:/usr/local/games:/snap/bin");
	free(joined);
	pwd->next = shlvl;
	shlvl->next = oldpwd;
	oldpwd->next = underscore;
	underscore->next = path;
	if (path)
		path->next = NULL;
}

static t_env	*setup_env_chain(char *cwd)
{
	t_env	*pwd;
	t_env	*shlvl;
	t_env	*oldpwd;
	t_env	*underscore;

	pwd = new_env("PWD", cwd, 1, 1);
	shlvl = new_env("SHLVL", "1", 1, 1);
	oldpwd = new_env("OLDPWD", NULL, 0, 1);
	underscore = new_env("_", "/usr/bin/env", 1, 0);
	if (!pwd || !shlvl || !oldpwd || !underscore)
		return (NULL);
	link_env_nodes(pwd, shlvl, oldpwd, underscore);
	return (pwd);
}

t_env	*init_minimal_env(t_shell *shell)
{
	char	cwd[1024];

	shell->shell_lvl = 1;
	(void)shell;
	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	return (setup_env_chain(cwd));
}
