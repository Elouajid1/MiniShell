/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-aid <moel-aid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:46:31 by moel-aid          #+#    #+#             */
/*   Updated: 2024/11/17 21:38:40 by moel-aid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>

int		ft_isalpha(int a);
int		ft_isdigit(int a);
int		ft_isalnum(int a);
size_t	ft_strlen(const char *str);
int		ft_atoi(const char *str);
char	*ft_strchr(const char *s, int a);
int		ft_strcmp(char *s1, char *s2);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
char	**ft_split(char *s, char c);
void	*ft_memset(void *s, int a, size_t len);
#endif
