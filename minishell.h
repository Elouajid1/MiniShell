/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:40:55 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/06/28 17:07:47 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef    MINISHELL_H
# define    MINISHELL_H

# include "libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	T_WORD,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_PIPE,
	T_HEREDOC,
	T_APPEND,
} t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}t_token;

typedef enum s_redir_type
{
	R_OUTPUT,
	R_INPUT,
	R_APPEND,
	R_HEREDOC,
}t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}t_redir;

typedef	struct s_command
{
	char	**argv;
	t_redir	*redir;
	struct s_command	*next;
} t_command;

int			ft_strcmp(const char *s1, const char *s2);
char		*expand_str(char *str);
char		*rm_quotes(char	*str);
char		**ft_split(char const *s, char c);
void		tokenize(t_token **token, char **strs);
void		free_command(t_command **comm);
void		free_redir(t_redir **redir);
void		free_tokens(t_token **token);
t_command	*parse(t_command **comm, t_token *token);


# endif