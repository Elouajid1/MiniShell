/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:08:38 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/11 13:33:43 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include "minishell.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/stat.h>
# define BUFFER_SIZE 42

typedef enum s_redir_type
{
	R_OUTPUT,
	R_INPUT,
	R_APPEND,
	R_HEREDOC,
}					t_redir_type;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_redir
{
	int				type;
	char			*file;
	int				heredoc_fd;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redir;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				show_in_export;
	int				show_in_env;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env			*env;
	t_cmd			*cmds;
	t_token			*token;
	int				status;
	int				last_status;
	bool			printed;
	int				i;
	// int				d_quotes;
	// int				s_quotes;
	int				count;
	int				last_exit_code;
	bool			interactive;
	int				sig;
	int				wait_result;
	int				single_pid;
	int				flag;
	int				pipeline_count;
	int				g_kill_pipeline;
	int				pipeline_pids[256];
	int				env_flag;
	int				shell_lvl;
}					t_shell;

/* ************************************************************************** */
/*                              Parsing FUNCTIONS                             */
/* ************************************************************************** */

char				*expand_str(char *str, t_env *env);
char				*rm_quotes(char *str);
void				expand(t_token **token, t_env *env);
void				tokenize(t_token **token, char **strs);
void				free_command(t_cmd **comm);
void				free_redir(t_redir **redir);
void				free_tokens(t_token **token);
t_cmd				*parse(t_cmd **comm, t_token *token);
char				**ft_split1(char const *s, char c);
void				q_rremoval(t_token **token);
void				q_removal(t_token **token);

/* ************************************************************************** */
/*                              BUILT IN FUNCTIONS                            */
/* ************************************************************************** */

int					builtin_cd(char **argv, t_shell *shell);
int					builtin_env(t_env *env, t_shell *shell);
int					builtin_export(char **argv, t_shell *shell);
int					builtin_exit(char **argv, t_shell *shell);
int					builtin_unset(char **argv, t_shell *shell);
int					builtin_echo(char **argv);
int					builtin_pwd(void);

/* ************************************************************************** */
/*                              EXECUTION FUNCTIONS                           */
/* ************************************************************************** */

int					execute_commands(t_shell *shell);
int					execute_builtin(t_cmd *cmd, t_shell *shell);
int					execute_pipeline(t_cmd *cmds, t_shell *shell);
int					execute_single_command(t_cmd *cmd, t_shell *shell);
int					fork_and_execute(char *path, char **argv, char **envp,
						t_shell *shell);
int					wait_for_children(t_shell *shell);
void				execute_child_command(t_cmd *cmd, t_shell *shell);
void				setup_parent_process(int *prev_fd, int pipe_fds[2],
						bool has_next);
void				setup_child_process(int prev_fd, int pipe_fds[2]);
int					handle_fork_and_exec(t_cmd *current, int prev_fd,
						int *pipe_fds, t_shell *shell);
int					process_pipeline_commands(t_cmd *cmds, t_shell *shell);
int					handle_heredoc(char *delimiter, t_shell *shell, int *fd);
int					execute_builtin(t_cmd *cmd, t_shell *shell);
int					process_heredocs(t_cmd *cmds, t_shell *shell);

/* ************************************************************************** */
/*                              Free FUNCTIONS                                */
/* ************************************************************************** */

void				free_array(char **array);
void				free_env_list(t_env *env);
void				free_redir_list(t_redir *redir);
void				free_cmd_list(t_cmd *cmds);
void				cleanup_shell(t_shell *shell, bool flag);

/* ************************************************************************** */
/*                              Envirenements FUNCTIONS                       */
/* ************************************************************************** */

t_env				*array_to_env(char **env, t_shell *shell);
char				*get_env_value(t_env *env, char *key);
t_env				*create_env_node(char *key, char *value);
void				add_env_node(t_env **env, t_env *new_node);
char				*find_env_key(t_env *env, char *key);
char				**env_to_array(t_env *env);
char				*create_env_string(t_env *env_node);
int					count_env_with_values(t_env *env);

/* ************************************************************************** */
/*                              Signals FUNCTIONS                             */
/* ************************************************************************** */

void				setup_signals(void);
void				setup_child_signals(void);
void				handle_sigint_pipeline(int sig);
void				handle_sigint_prompt(int sig);
void				handle_sigquit(int sig);
int					handle_single_child_signals(int status);
void				handle_sigquit_single(int sig);
void				handle_sigint_single(int sig);
void				signal_reset_prompt(int sig);
void				set_signals_interactive(void);
void				signal_print_newline(int signal);
void				set_signals_noninteractive(void);
void				ignore_sigquit(void);


char				*permission_error(char *cmd, t_shell *shell);
char				*no_such_file_error(char *cmd, t_shell *shell);


/* ************************************************************************** */
/*                              UTILITY FUNCTIONS                             */
/* ************************************************************************** */

bool				is_builtin(char *cmd);
char				*get_executable_path(char *cmd, t_shell *shell);
void				free_array(char **split);
int					setup_redirections(t_redir *redir);
char				*get_env_value(t_env *env, char *key);
t_env				*create_env_node(char *key, char *value);
void				add_env_node(t_env **env, t_env *new_node);
int					export_variable(t_shell *shell, char *arg);
void				unset_variable(t_shell *shell, char *key);
t_env				*find_env_node(t_env *env, char *key);
int					print_export_list(t_env *env, t_shell *shell);
bool				is_numeric(char *str);
bool				is_valid_identifier(char *str);
void				setup_child_signals(void);
void				begin_child_execution(t_cmd *current, int prev_fd,
						int *pipe_fds, t_shell *shell);
int					handle_command_error(char *cmd_name);
int					pipeline_loop(t_cmd *cmds, t_shell *shell);
int					handle_pipe_creation(t_cmd *current, int *pipe_fds);
bool				is_numeric(char *str);
bool				is_valid_identifier(char *str);
int					absolute_path(char *cmd);
int					handle_path_error(char *cmd_name);
int					error(char *str);
int					syntax_errors(t_token *token);
int					handle_builtin_logic(t_cmd *cmd, t_shell *shell);
void				restore_stdio(int backup[2]);
int					backup_stdio(int backup[2]);
void				add_defaults_env(t_env **env);
t_env				*new_env_node(char *key, char *value);
int					print_list(t_env *env);
int					print_export_list1(t_env *env);
char				*get_next_line(int fd);
t_env				*init_minimal_env(t_shell *shell);
char				*get_heredoc_expansion(char *str, t_env *env);
int					env_error_message(char *path);
int					check_for_empty(char **splited);
char				*ft_strncpy(char *dest, char *src, unsigned int n);
char				*ft_strcpy(char *dest, char *src);
char				*ft_strcat(char *dest, char *src);
int					return_error_code(t_shell *shell);

#endif
