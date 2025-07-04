/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 22:02:45 by moel-aid          #+#    #+#             */
/*   Updated: 2025/03/06 14:48:17 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

void	check_for_command_path(char *cmd, char **env, int ac)
{
	if (!(*env))
		return ;
	else if (ac != 5)
		exit_handler(1);
	else if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == -1)
		{
			ft_putstr_fd("./pipex : no such file or directory: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}
	else
		return ;
}

void	execution(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	path = get_path(s_cmd[0], env);
	if (execve(path, s_cmd, env) == -1)
	{
		ft_putstr_fd("pipex: command not found:", 2);
		ft_putendl_fd(s_cmd[0], 2);
		free_all_tab(s_cmd);
		exit(0);
	}
}

void	child(char **av, int *pipe_table, char **env)
{
	int		fd;

	fd = open_file(av[1], 0);
	if (fd > 0)
	{
		dup2(fd, 0);
		dup2(pipe_table[1], 1);
		close(pipe_table[0]);
		execution(av[2], env);
	}
	else if (fd < 0)
	{
		ft_putstr_fd("pipex: no such file or directory: ", 2);
		ft_putstr_fd(av[1], 2);
	}
}

void	child2(char **av, int *pipe_table, char **env)
{
	int		fd;

	fd = open_file(av[4], 1);
	if (fd == -1)
		exit_handler(4);
	dup2(fd, 1);
	dup2(pipe_table[0], 0);
	close(pipe_table[1]);
	execution(av[3], env);
}

int	main(int ac, char **av, char **env)
{
	int		pipe_table[2];
	pid_t	pid;
	pid_t	pid2;

	if ((av[1][0] == '\0') || (av[2][0] == '\0') || (av[3][0] == '\0'))
		exit_handler(2);
	check_files(av);
	check_for_command_path(av[2], env, ac);
	check_for_command_path(av[3], env, ac);
	if (pipe(pipe_table) == -1)
		exit(-1);
	pid = fork();
	if (pid)
		pid2 = fork();
	if (pid == -1)
		exit(-1);
	if (!pid)
		child(av, pipe_table, env);
	else if (!pid2)
		child2(av, pipe_table, env);
	else
		wait(NULL);
}
