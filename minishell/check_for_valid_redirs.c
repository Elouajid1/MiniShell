/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_valid_redis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:50:33 by moel-aid          #+#    #+#             */
/*   Updated: 2025/06/26 16:50:36 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_redirs_type(t_shell *shell)
{
    t_redir *current;
    int     count;

    count = 1;
    current = shell->cmds->redir;
    if (current && current->next)
    {
        if (current->type == T_REDIR_OUT && current->next->type == T_REDIR_IN)
            return (-1);
    }
    while (current)
    {
        current = current->next;
        count++;
    }
    if (count >= 3)
        return (-1);
    return (1);
}

char print_redir_error_message(t_shell *shell)
{
    t_redir *current;

    current = shell->cmds->redir;
    while (current)
        current = current->next;
    if (current->type == T_REDIR_IN)
        return ('<');
    else if (current->type == T_REDIR_OUT)
        return ('>');
}

int check_redirs(t_shell *shell)
{
    if (check_redirs_type(shell) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("syntax error near unexpected ", 2);
        ft_putstr_fd("token '%c'\n", print_redir_error_message(shell));
        cleanup_shell(shell);
        return ()
    }
}