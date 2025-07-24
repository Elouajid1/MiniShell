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

// This function applies the heredoc redirection in the command's child process.
// It should be called by setup_redirections for R_HEREDOC type.
int apply_heredoc_redirection(char *heredoc_file_path)
{
    int fd;

    fd = open(heredoc_file_path, O_RDONLY);
    if (fd == -1)
    {
        perror("open heredoc temp file for read");
        return (-1); // Indicate error
    }
    dup2(fd, STDIN_FILENO); // Redirect STDIN to the heredoc temp file
    close(fd); // Close the original FD after duplication

    // Unlink the file immediately in the child. It will remain accessible via FD 0.
    // The actual `heredoc_file_path` string (the path) that was ft_strdup'd in the parser
    // must be freed later by your `free_command` or similar cleanup.
    unlink(heredoc_file_path);
    return (0); // Success
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
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h> // For add_history, if needed

// Assuming your global g_last_exit_code and ft_ functions are available
extern int g_last_exit_code; // Assuming this is already declared elsewhere

// Signal handler for the PARENT shell during heredoc input collection
// This is the same as your `signal_reset_prompt` but explicitly for this phase.
void    heredoc_parent_sigint_handler(int sig)
{
    (void)sig;
    g_last_exit_code = 130; // Conventionally 128 + SIGINT (2)
    write(STDERR_FILENO, "\n", 1); // Print newline for a clean break
    rl_on_new_line();
    rl_replace_line("", 0); // Clear the current input line
    rl_redisplay(); // Show a fresh prompt
    // Do NOT exit here, just reset readline and let the parent loop continue
}

// Your existing heredoc_loop (remains mostly the same, now called by collect_heredoc_input)
// This loop runs in the PARENT process.
void heredoc_loop(char *line, char *delimiter, t_shell *shell, int fd)
{
    char    *expanded_line;
    char    *deli;

    // Set signals for interactive readline in the parent
    // The parent's main loop's signal setup should be temporarily overridden.
    struct sigaction sa_int;
    struct sigaction sa_old_int;

    ft_memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = heredoc_parent_sigint_handler;
    sigaction(SIGINT, &sa_int, &sa_old_int); // Set custom handler, save old

    // Also ignore SIGQUIT in the parent during readline
    struct sigaction sa_quit;
    struct sigaction sa_old_quit;
    ft_memset(&sa_quit, 0, sizeof(sa_quit));
    sa_quit.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa_quit, &sa_old_quit); // Ignore SIGQUIT, save old

    while (1)
    {
        line = readline("> ");
        
        // If g_last_exit_code was set to 130 by the signal handler, it means Ctrl+C
        if (g_last_exit_code == 130) {
            free(line); // readline might return a valid line even after a signal
            break; // Exit the loop due to interruption
        }

        if (!line) // EOF (Ctrl+D)
        {
            printf("\n"); // Print newline for EOF
            break ;
        }
        
        deli = rm_quotes(delimiter); // Ensure rm_quotes is memory safe
        if (ft_strcmp(deli, line) == 0)
        {
            free(line);
            if (deli) free(deli);
            break ;
        }
        expanded_line = evaluate_heredoc_line(line, shell, delimiter);
        if (expanded_line)
            ft_putendl_fd(expanded_line, fd);
        free(line);
        if (expanded_line != line)
            free(expanded_line);
        if (deli) free(deli);
    }
    
    // Restore original signal handlers after heredoc input is done
    sigaction(SIGINT, &sa_old_int, NULL);
    sigaction(SIGQUIT, &sa_old_quit, NULL);
}


// This function is for collecting heredoc input from the user in the parent process.
// It should be called by your parser when it encounters '<<'.
// It returns the path to the temporary heredoc file, or NULL if interrupted.
char *collect_heredoc_input(char *delimiter, t_shell *shell)
{
    char    *line = NULL; // Initialize line
    int     fd;
    char    *tmp_file_path;

    // Generate a UNIQUE temporary file name
    char tmp_template[] = "/tmp/minishell_heredoc_XXXXXX";
    fd = mkstemp(tmp_template); // mkstemp creates a unique file and opens it for R/W
    if (fd == -1)
    {
        perror("mkstemp for heredoc");
        return (NULL);
    }
    tmp_file_path = ft_strdup(tmp_template); // Store the generated unique name
    if (!tmp_file_path) // Handle strdup failure
    {
        close(fd);
        unlink(tmp_template); // Clean up the created temp file
        perror("ft_strdup");
        return (NULL);
    }

    // Reset g_last_exit_code before starting heredoc input
    // This is important because the signal handler might set it.
    g_last_exit_code = 0; 

    // Execute the heredoc input loop (runs in parent)
    heredoc_loop(line, delimiter, shell, fd);

    close(fd); // Close the write end of the temp file

    // Check if heredoc_loop was interrupted by Ctrl+C
    if (g_last_exit_code == 130) {
        unlink(tmp_file_path); // Clean up the temp file
        free(tmp_file_path);
        g_last_exit_code = 0; // Reset for next command
        return (NULL); // Indicate interruption
    }

    return (tmp_file_path); // Return the path. The parser will store it.
                            // This string needs to be freed later (e.g., during t_cmd cleanup).
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
