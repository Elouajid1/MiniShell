/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:35:17 by moel-aid          #+#    #+#             */
/*   Updated: 2025/07/22 13:35:20 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char    *print_double_dot_error(t_shell *shell)
{
    ft_putstr_fd("minishell: .: filename argument required\n",
        STDERR_FILENO);
    ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
    shell->flag = -2;
    return (NULL);
}

