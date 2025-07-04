/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:08:38 by moel-aid          #+#    #+#             */
/*   Updated: 2025/05/28 11:14:51 by moel-aid         ###   ########.fr       */
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

typedef struct s_global
{
	int				signal_flag;
	pid_t			pipeline_pids[256];
	int				pipeline_count;
	volatile int	g_kill_pipeline;
	int				signal;
	bool			interactive;
	pid_t			single_pid;
	int 			last_exit_status;
}					t_global;

typedef struct s_redir
{
	int				type;
	char			*file;
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
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env			*env;
	t_cmd			*cmds;
	t_cmd			*command;
	t_token			*token;
	int				status;
	int				last_status;
	bool			printed;
	int				i;
	int				last_exit_code;
	bool			interactive;
	int				sig;
	int				wait_result;
}					t_shell;

/* ************************************************************************** */
/*                              Parsing FUNCTIONS                             */
/* ************************************************************************** */

char				*expand_str(char *str, t_env *env);
char				*rm_quotes(char	*str);
void				expand(t_token **token, t_env *env);
void				tokenize(t_token **token, char **strs);
void				free_command(t_cmd **comm);
void				free_redir(t_redir **redir);
void				free_tokens(t_token **token);
t_cmd				*parse(t_cmd **comm, t_token *token);

/* ************************************************************************** */
/*                              BUILT IN FUNCTIONS                            */
/* ************************************************************************** */

int					builtin_cd(char **argv, t_shell *shell);
int					builtin_env(t_env *env);
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
int					fork_and_execute(char *path, char **argv, char **envp);
int					wait_for_children(t_shell *shell);
void				execute_child_command(t_cmd *cmd, t_shell *shell);
void				setup_parent_process(int *prev_fd, int pipe_fds[2],
						bool has_next);
void				setup_child_process(t_cmd *cmd, int prev_fd,
						int pipe_fds[2]);
int					handle_fork_and_exec(t_cmd *current, int prev_fd,
						int *pipe_fds, t_shell *shell);
int					process_pipeline_commands(t_cmd *cmds, t_shell *shell);
int					handle_heredoc(char *delimiter);
int					execute_builtin(t_cmd *cmd, t_shell *shell);

/* ************************************************************************** */
/*                              Free FUNCTIONS                                */
/* ************************************************************************** */

void				free_array(char **array);
void				free_env_list(t_env *env);
void				free_redir_list(t_redir *redir);
void				free_cmd_list(t_cmd *cmds);
void				cleanup_shell(t_shell *shell);

/* ************************************************************************** */
/*                              Envirenements FUNCTIONS                       */
/* ************************************************************************** */

t_env				*array_to_env(char **env);
char				*get_env_value(t_env *env, char *key);
t_env				*create_env_node(char *key, char *value);
void				add_env_node(t_env **env, t_env *new_node);

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

/* ************************************************************************** */
/*                              UTILITY FUNCTIONS                             */
/* ************************************************************************** */

bool				is_builtin(char *cmd);
char				*get_executable_path(char *cmd);
void				free_array(char **split);
int					setup_redirections(t_redir *redir);
char				**env_to_array(t_env *env);
char				*get_env_value(t_env *env, char *key);
t_env				*create_env_node(char *key, char *value);
void				add_env_node(t_env **env, t_env *new_node);
int					export_variable(t_shell *shell, char *arg);
void				unset_variable(t_shell *shell, char *key);
t_env				*find_env_node(t_env *env, char *key);
int					print_export_list(t_env *env);
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
#endif
