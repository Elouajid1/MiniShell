/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:37:29 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/27 17:37:33 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_last_exit_code;

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (str[i - 1] == '"' && str[0] == '"')
		return (1);
	if (str[i - 1] == 39 && str[0] == 39)
		return (1);
	return (0);
}

char	*evaluate_heredoc_line(char *line, t_shell *shell, char *delimiter)
{
	if (has_quotes(delimiter))
		return (ft_strdup(line));
	else if (!has_quotes(delimiter))
		return (get_heredoc_expansion(line, shell->env));
	return (NULL);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h> // For open flags O_RDONLY, O_CREAT, O_WRONLY, O_TRUNC
#include <readline/readline.h>
// Include your custom headers (ft_strdup, ft_strcmp, etc.)

// --- New/Refactored Signal Handler for the Forked Heredoc Child ---
// This handler will be used *only* by the child process created within handle_heredoc.
void    heredoc_child_reader_sigint_handler(int sig)
{
    (void)sig;
    // Clean up readline's internal state
    rl_on_new_line();
    rl_replace_line("", 0); // Clear the current input line
    // Do NOT call rl_redisplay() here, it can cause issues when exiting
    write(STDERR_FILENO, "\n", 1); // Print a newline for a clean exit
    exit(130); // Exit with status 130 to indicate interruption
}

// --- Your existing heredoc_loop (remains mostly the same, now called by the forked child) ---
// Note: Removed internal signal calls from here, as fork handles it.
void heredoc_loop(char *line, char *delimiter, t_shell *shell, int fd)
{
    char    *expanded_line;
    char    *deli;

    while (1)
    {
        // Signal handling is external to this loop (in the forked child).
        line = readline("> ");
        
        if (!line) // readline returns NULL on EOF (Ctrl+D) or on SIGINT (Ctrl+C)
        {
            printf("\n"); // Print newline for EOF, or after Ctrl+C for a clean break
            break ;
        }
        
        deli = rm_quotes(delimiter); // Ensure rm_quotes is memory safe (allocates or returns original)
        if (ft_strcmp(deli, line) == 0)
        {
            free(line);
            if (deli) free(deli); // Assuming rm_quotes allocates deli
            break ;
        }
        expanded_line = evaluate_heredoc_line(line, shell, delimiter);
        if (expanded_line)
            ft_putendl_fd(expanded_line, fd);
        free(line);
        if (expanded_line != line) // Only free if it's a new allocation
            free(expanded_line);
        if (deli) free(deli); // Free deli if rm_quotes allocates
    }
}

// --- Modified handle_heredoc ---
// This function will now handle everything for heredocs at execution time.
// It will be called from setup_redirections.
// It must return 0 for success (redirection set up) or -1 for failure (interrupted or error).
int handle_heredoc(char *delimiter_str, t_shell *shell)
{
    char    *tmp_file_path;
    int     pipe_to_child_fds[2]; // Pipe for status communication
    pid_t   heredoc_pid;
    int     status;
    int     temp_fd; // File descriptor for the temporary heredoc file
    struct sigaction sa_old_int, sa_old_quit; // To save/restore parent's sigaction

    // 1. Create a unique temporary file path (not yet opened for writing in parent)
    char tmp_template[] = "/tmp/minishell_heredoc_XXXXXX";
    // We only need the name, `open` later will create/open.
    // Use `mkstemp` to get a guaranteed unique name and an open FD for a moment, then close it.
    // Or, just use a simple unique name for now if it's always created in child.
    temp_fd = mkstemp(tmp_template);
    if (temp_fd == -1) {
        perror("mkstemp for heredoc temp file name");
        return -1;
    }
    close(temp_fd); // Close the FD created by mkstemp, we just need the unique name.
    tmp_file_path = ft_strdup(tmp_template);
    if (!tmp_file_path) {
        perror("ft_strdup for heredoc path");
        unlink(tmp_template); // Clean up in case of strdup failure
        return -1;
    }

    // 2. Create a pipe to communicate exit status from the heredoc child to this process
    if (pipe(pipe_to_child_fds) == -1) {
        perror("pipe for heredoc status");
        free(tmp_file_path);
        return -1;
    }

    // 3. Save current signal handlers and set to ignore in the current process
    // (This current process is the command's child process that will fork for heredoc)
    struct sigaction sa_ignore;
    ft_memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN; // Ignore signal

    sigaction(SIGINT, &sa_ignore, &sa_old_int);
    sigaction(SIGQUIT, &sa_ignore, &sa_old_quit);

    // 4. Fork the heredoc input reader child
    heredoc_pid = fork();
    if (heredoc_pid == -1)
    {
        perror("fork for heredoc input reader");
        close(pipe_to_child_fds[0]);
        close(pipe_to_child_fds[1]);
        unlink(tmp_file_path);
        free(tmp_file_path);
        sigaction(SIGINT, &sa_old_int, NULL);
        sigaction(SIGQUIT, &sa_old_quit, NULL);
        return (-1);
    }
    else if (heredoc_pid == 0) // Heredoc input reader child process
    {
        // Close read end of pipe to parent, as child will only write status
        close(pipe_to_child_fds[0]); 

        // Set child-specific signal handlers for readline
        struct sigaction sa_child_int;
        ft_memset(&sa_child_int, 0, sizeof(sa_child_int));
        sa_child_int.sa_handler = heredoc_child_reader_sigint_handler;
        sigaction(SIGINT, &sa_child_int, NULL); // Ctrl+C exits this child
        signal(SIGQUIT, SIG_IGN); // Child ignores SIGQUIT

        // Open the temporary file for writing (O_TRUNC to ensure it's empty)
        temp_fd = open(tmp_file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (temp_fd == -1) {
            perror("open heredoc temp file in child");
            // Communicate error back to parent via pipe (e.g., write -1)
            int err_code = -1;
            write(pipe_to_child_fds[1], &err_code, sizeof(err_code));
            close(pipe_to_child_fds[1]);
            unlink(tmp_file_path);
            free(tmp_file_path);
            exit(1); // Exit with a non-zero status
        }

        // Execute the heredoc input loop
        heredoc_loop(NULL, delimiter_str, shell, temp_fd);

        close(temp_fd); // Close the write end of the temp file

        // Communicate success (0) back to parent via pipe
        int success_code = 0;
        write(pipe_to_child_fds[1], &success_code, sizeof(success_code));
        close(pipe_to_child_fds[1]);
        
        exit(0); // Child exits normally
    }
    else // Current process (the command's child, which is parent to heredoc reader)
    {
        // Close write end of pipe, as this process will only read status
        close(pipe_to_child_fds[1]);

        // Wait for the heredoc input reader child
        waitpid(heredoc_pid, &status, 0);

        // Restore signal handlers in the current process
        sigaction(SIGINT, &sa_old_int, NULL);
        sigaction(SIGQUIT, &sa_old_quit, NULL);

        int heredoc_status_from_child = 0;
        // Read status from the pipe
        if (read(pipe_to_child_fds[0], &heredoc_status_from_child, sizeof(heredoc_status_from_child)) == -1) {
            perror("read from heredoc status pipe");
            close(pipe_to_child_fds[0]);
            unlink(tmp_file_path);
            free(tmp_file_path);
            return -1; // Indicate error
        }
        close(pipe_to_child_fds[0]);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
        {
            // Heredoc was interrupted by Ctrl+C
            shell->last_exit_code = 130; // Update shell's last exit code
            unlink(tmp_file_path); // Clean up the temp file
            free(tmp_file_path);
            return (-1); // Indicate failure/interruption
        }
        else if (heredoc_status_from_child == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            // Child had an error (e.g., open failed) or abnormal termination
            unlink(tmp_file_path);
            free(tmp_file_path);
            return (-1); // Indicate general failure
        }

        // --- If heredoc input collection was successful ---
        // Open the temporary file for reading
        int read_fd = open(tmp_file_path, O_RDONLY);
        if (read_fd == -1)
        {
            perror("open heredoc temp file for command input");
            unlink(tmp_file_path);
            free(tmp_file_path);
            return (-1);
        }
        dup2(read_fd, STDIN_FILENO); // Redirect STDIN to the heredoc temp file
        close(read_fd); // Close the original FD after duplication

        // Unlink the file immediately after dup2 in this process.
        // It will remain accessible via FD 0 (STDIN_FILENO) until this process exits.
        unlink(tmp_file_path); // Delete the temporary file from disk
        free(tmp_file_path); // Free the path string

        return (0); // Success: STDIN is now redirected to heredoc content
    }
}

// void	heredoc_loop(char *line, char *delimiter, t_shell *shell, int fd)
// {
// 	char	*expanded_line;
// 	char	*deli;

// 	while (1)
// 	{
// 		set_signals_interactive();
// 		line = readline("> ");
// 		set_signals_noninteractive();
// 		if (!line)
// 		{
// 			printf("\n");
// 			break ;
// 		}
// 		deli = rm_quotes(delimiter);
// 		if (ft_strcmp(deli, line) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		expanded_line = evaluate_heredoc_line(line, shell, delimiter);
// 		if (expanded_line)
// 			ft_putendl_fd(expanded_line, fd);
// 		free(line);
// 		if (expanded_line != line)
// 			free(expanded_line);
// 	}
// }

// int	read_heredoc(int fd, char *path, t_shell *shell)
// {
// 	int	read_fd;

// 	if (shell->cmds->argv[1])
// 	{
// 		close(fd);
// 		unlink(path);
// 		return (0);
// 	}
// 	read_fd = open(path, O_RDONLY);
// 	if (read_fd == -1)
// 	{
// 		perror("open");
// 		close(fd);
// 		unlink(path);
// 		return (-1);
// 	}
// 	dup2(read_fd, STDIN_FILENO);
// 	close(read_fd);
// 	close(fd);
// 	unlink(path);
// 	return (0);
// }

// int	handle_heredoc(char *delimiter, t_shell *shell)
// {
// 	char	*line;
// 	int		fd;
// 	char	*tmp_file;

//     line = NULL;
// 	tmp_file = "/tmp/minishell_heredoc";
// 	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		return (-1);
// 	}
// 	heredoc_loop(line, delimiter, shell, fd);
// 	return (read_heredoc(fd, tmp_file, shell));
// }
