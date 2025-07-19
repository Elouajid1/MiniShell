/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:15:40 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 19:15:43 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISREG(st.st_mode))
		return (access(path, X_OK) == 0);
	return (0);
}

char	*extract_directory(char *path_copy, int start, int len)
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