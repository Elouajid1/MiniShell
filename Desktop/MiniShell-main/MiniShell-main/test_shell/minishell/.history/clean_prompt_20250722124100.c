/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:12:50 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/16 11:12:54 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    clean_prompt(t_shell *shell)
{
    if (!shell)
        return ;
    if (shell->cmds)
        free_cmd_list(shell->cmds);
    if (shell->cmds->redir)
        free_redir_list(shell->cmds->redir);
    if (shell->token)
        free_tokens(&(shell->token));
}