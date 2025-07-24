/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:40:27 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/19 22:01:41 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand(t_token **token, t_env *env)
{
	t_token	*head;
	char	*expanded;

	head = (*token);
	while (head)
	{
		if (head->type == T_WORD)
		{
			expanded = expand_str(head->value, env);
			free(head->value);
			head->value = expanded;
		}
		head = head->next;
	}
}

int	error(char *str)
{
	printf("%s\n", str);
	return (1);
}

int	syntax_errors(t_token *token)
{
	t_token	*head;

	head = token;
	if (head->type == T_PIPE)
	{
		printf("%s\n", "Syntax Error! pipe first");
		return (1);
	}
	while (head)
	{
		if (head->type == T_APPEND || head->type == T_REDIR_IN
			|| head->type == T_REDIR_OUT || head->type == T_HEREDOC)
		{
			if (head->next == NULL || head->next->type != T_WORD)
			{
				printf("%s\n", "Syntax Error! redir");
				return (1);
			}
		}
		else if (head->type == T_PIPE)
		{
			if (head->next == NULL)
			{
				printf("%s\n", "Syntax Error! pipe last");
				return (1);
			}
			else if (head->next->type == T_PIPE)
			{
				printf("%s\n", "Syntax Error! pipe after pipe");
				return (1);
			}
			head = head->next;
		}
		head = head->next;
	}
	return (0);
}

// int main()
// {
// 	char *str;
// 	t_token *token;
// 	t_cmd *comm;
// 	t_cmd *head;
// 	char **strs;

// 	token = NULL;
// 	comm = NULL;
// 	while (1)
// 	{
// 		str = readline("Minishell$ ");
// 		add_history(str);
// 		strs = ft_split1(str, 32);
// 		tokenize(&token, strs);
// 		syntax_errors(token);
// 		expand(&token, env);
// 		head = parse(&comm, token);
// 		int i = 0;
// 		while (head)
// 		{
// 			// printf("%d\n", head->redir->type);
// 			i = 0;
// 			while (head->argv[i])
// 			{
// 				printf("%s\n", head->argv[i]);
// 				i++;
// 			}
// 			head = head->next;
// 		}
// 		free(str);
// 		free_tokens(&token);
// 		free_command(&comm);
// 		token = NULL;
// 		comm = NULL;
// 	}
// }
