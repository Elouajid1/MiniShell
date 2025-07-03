/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:54:55 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/02 14:58:44 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*rm_quotes(char	*str)
{
	int	i;
	int	j;
	int	single_q;
	int	double_q;
	char	*res;

	i = 0;
	j = 0;
	single_q = 0;
	double_q = 0;
	res = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] == 34 && !single_q)
		{
			if (!double_q)
				double_q = 1;
			else
				double_q = 0;
			i++;
		}
		else if (str[i] == 39 && !double_q)
		{
			if (!single_q)
				single_q = 1;
			else
				single_q = 0;
			i++;
		}
		else
		{
			res[j] = str[i];
			i++;
			j++;
		}	
	}
	res[j] = 0;
	return (res);
}

char	*expand_str(char *str)
{
	int	i;
	int	j;
	char	*value;
	char	*var; 
	char	*res;
	char	*tmp;
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	i = 0;
	j = 0;
	res = ft_strdup("");
	var = NULL;
	value = NULL;
	while (str[i])
	{
		if (str[i] == 34 && !in_single)
		{
			if (!in_double)
				in_double = 1;
			else
				in_double = 0;
			i++;
		}
		else if (str[i] == 39 && !in_double)
		{
			if (!in_single)
				in_single = 1;
			else
				in_single = 0;
			i++;
		}
		else if (str[i] == '$' && in_single == 0)
		{
			i++;
			if (str[i] == '$')
			{
				tmp = res;
				value = ft_itoa(getpid());
				res = ft_strjoin(res, value);
				free (value);
				free (tmp);
				i++;
			}
			else if (str[i] == '?')
			{
				tmp = res;
				res = ft_strjoin(res, "");
				free (tmp);
				i++;
			}
			else
			{
				j = i;
			while (str[i] && isalnum(str[i]))
				i++;
			value = ft_substr(str, j, i - j);
			var = getenv(value);
			if (!var)
				var = "";
			tmp = res;	
			res = ft_strjoin(res, var);
			free (tmp);
			free (value);
			}
		}
		else if (str[i] == '$' && in_single == 1)
		{
			tmp = res;
			res = ft_strjoin(res, "$");
			free (tmp);
			i++;
		}
		else
		{
			char	temp[2];
			temp[0] = str[i];
			temp[1] = '\0';
			tmp = res;
			res = ft_strjoin(res, temp);
			free (tmp);
			i++;
		}
	}
	if (in_double == 1 || in_single == 1)
	{
		printf("Syntax Error!\n");
		return (NULL);
	}
	return (res);
}

// int main()
// {
// 	char	*str = "wtre\'g'r \"$USER\" hfkjsb \"dsg'kfj'\" sdbfsd";
// 	char	*var;
// 	var = expand_str(str);
// 	// char *res = rm_quotes(str);
// 	printf("%s\n", var);
// }