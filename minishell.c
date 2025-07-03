/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:40:27 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/06/30 18:12:23 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand(t_token **token)
{
	t_token	*head;

	head = (*token);
	while (head)
	{
		if (head->type == T_WORD)
			head->value = expand_str(head->value);
		head = head->next;
	}
}

int main()
{
	char *str;
	t_token *token;
	t_command *comm;
	t_command *head;
	char **strs;

	token = NULL;
	comm = NULL;
	while (1)
	{
		str = readline("Minishell$ ");
		add_history(str);
		strs = ft_split(str, 32);
		tokenize(&token, strs);
		expand(&token);
		head = parse(&comm, token);
		int i = 0;
		while (head)
		{
			// printf("%d\n", head->redir->type);
			i = 0;
			while (head->argv[i])
			{
				printf("%s\n", head->argv[i]);
				i++;
			}
			head = head->next;
		}
		free(str);
		free_tokens(&token);
		free_command(&comm);
		token = NULL;
		comm = NULL;
	}
}