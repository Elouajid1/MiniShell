/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 11:21:19 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/30 11:00:54 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[j] != '\0')
	{
		j++;
	}
	while (src[i])
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

int	has_only_n(char *arg)
{
	int	i;

	i = 1;
	if (ft_strlen(arg) == 1 && arg[0] == '-')
		return (1);
	if (arg[0] != '-')
		return (1);
	if (arg[0] == '-')
	{
		while (arg[i])
		{
			if (arg[i] != 'n')
				return (1);
			i++;
		}
	}
	return (0);
}

void	print_echo(char **argv, int *newline, t_shell *shell)
{
	shell->i = 1;
	while (argv[shell->i])
	{
		while (argv[shell->i] && !has_only_n(argv[shell->i]))
			shell->i++;
		if (shell->i > 1)
			*newline = 1;
		while (argv[shell->i])
		{
			ft_putstr_fd(argv[shell->i], STDOUT_FILENO);
			if (argv[shell->i + 1])
				write(1, " ", 1);
			shell->i++;
		}
	}
}
