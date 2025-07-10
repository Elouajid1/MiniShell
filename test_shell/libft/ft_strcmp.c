/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <moel-aid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:51:05 by moel-aid          #+#    #+#             */
/*   Updated: 2024/07/01 14:27:34 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' && s2[i] != '\0'))
		i++;
	if (s1[i] == s2[i] || (s1[i] == '\0' && s2[i] == '\0'))
		return (0);
	else
		return (s1[i] - s2[i]);
}
/*int main()
{
	char gogo[] = "";
	char fofo[] = "";
	printf("%d\n", ft_strcmp(gogo, fofo));
}*/
