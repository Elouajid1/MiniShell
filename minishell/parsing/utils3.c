/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:40:27 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/30 12:50:02 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	expand(t_token **token, t_env *env)
// {
// 	t_token	*head;
// 	char	*expanded;
// 	char	*old;

// 	head = (*token);
// 	while (head)
// 	{
// 		if (head->type == T_HEREDOC)
// 			head = head->next;
// 		else if (head->type == T_WORD && head->value)
// 		{
// 			old = head->value;
// 			expanded = expand_str(head->value, env);
// 			if (expanded)
// 			{
// 				head->value = expanded;
// 				free(old);
// 			}
// 		}
// 		head = head->next;
// 	}
// }

int	error(char *str)
{
	printf("%s\n", str);
	return (1);
}

int	redir_error(t_token *head)
{
	if (head->next == NULL || head->next->type != T_WORD)
	{
		printf("%s\n", "Syntax Error! near redir");
		return (1);
	}
	return (0);
}

int	pipe_error(t_token *head)
{
	if (head->next == NULL)
	{
		printf("%s\n", "Syntax Error! near pipe");
		return (1);
	}
	else if (head->next->type == T_PIPE)
	{
		printf("%s\n", "Syntax Error! near pipe");
		return (1);
	}
	return (0);
}

int	syntax_errors(t_token *token)
{
	t_token	*head;

	head = token;
	if (head->type == T_PIPE)
	{
		printf("%s\n", "Syntax Error! near pipe");
		return (1);
	}
	while (head)
	{
		if (head->type == T_APPEND || head->type == T_REDIR_IN
			|| head->type == T_REDIR_OUT || head->type == T_HEREDOC)
		{
			if (redir_error(head))
				return (1);
		}
		else if (head->type == T_PIPE)
		{
			if (pipe_error(head))
				return (1);
		}
		head = head->next;
	}
	return (0);
}
