/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 02:27:21 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/06/23 15:29:55 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*swp;

	if (!lst || !(*lst) || !del)
		return ;
	while (*lst)
	{
		swp = (*lst)-> next;
		ft_lstdelone(*lst, del);
		(*lst) = swp;
	}
}
