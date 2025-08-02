/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:15:40 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 14:53:50 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_pointers(t_shell *shell)
{
	shell->last_exit_code = 0;
	shell->command = NULL;
	shell->token = NULL;
	shell->cmds = NULL;
	shell->args = NULL;
}

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

char	*check_directory(char *dir, char *cmd)
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
