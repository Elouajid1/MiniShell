/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:03:02 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 18:03:05 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*create_full_path(char *dir, char *cmd)
{
	char	*full_path;
	int		dir_len;
	int		cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

static int	is_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISREG(st.st_mode))
		return (access(path, X_OK) == 0);
	return (0);
}

static char	*extract_directory(char *path_copy, int start, int len)
{
	char	*dir;

	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	ft_strncpy(dir, &path_copy[start], len);
	dir[len] = '\0';
	return (dir);
}

static char	*check_directory(char *dir, char *cmd)
{
	char	*full_path;

	full_path = create_full_path(dir, cmd);
	if (!full_path)
		return (NULL);
	if (is_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

static int	get_token_length(char *path_copy, int i, int start)
{
	if (path_copy[i] == ':')
		return (i - start);
	return (i - start + 1);
}

static char	*process_path_token(char *path_copy, int start, int len, char *cmd)
{
	char	*dir;
	char	*result;

	if (len == 0)
		return (NULL);
	dir = extract_directory(path_copy, start, len);
	if (!dir)
		return (NULL);
	result = check_directory(dir, cmd);
	free(dir);
	return (result);
}

static char	*search_in_path_dirs(char *path_copy, char *cmd)
{
	int		i;
	int		start;
	int		len;
	char	*result;

	i = 0;
	start = 0;
	while (path_copy[i])
	{
		if (path_copy[i] == ':' || path_copy[i + 1] == '\0')
		{
			len = get_token_length(path_copy, i, start);
			result = process_path_token(path_copy, start, len, cmd);
			if (result)
				return (result);
			start = i + 1;
		}
		i++;
	}
	return (NULL);
}

char	*search_in_path(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	*path_copy;
	char	*result;

	path_env = get_env_value(shell->env, "PATH");
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	result = search_in_path_dirs(path_copy, cmd);
	free(path_copy);
	return (result);
}