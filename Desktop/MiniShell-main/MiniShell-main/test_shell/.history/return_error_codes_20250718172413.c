/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_error_codes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:20:19 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/18 17:20:23 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int return_error_code(t_shell *shell)
{
    if (shell->flag == -2)
        return (2);
    if (shell->flag == -1)
        return (1);
    if (shell->flag == 0)
        return (0);
    if (shell->flag == 1)
        return (127);
    if (shell->flag == 2)
        return (127);
    if (shell->flag == 3)
        return (1);
    if (shell->flag == 4)
        return (126);
    return (127);
}

char    *return_error_message(int flag, char )
{

}