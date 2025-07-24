/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-ouaj <mel-ouaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:54:55 by mel-ouaj          #+#    #+#             */
/*   Updated: 2025/07/20 17:27:01 by mel-ouaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_last_exit_code;

int	q_handle(char c, int *double_q, int *single_q)
{
	if (c == 34 && !(*single_q))
	{
		*double_q = !*double_q;
		return (1);
	}
	else if (c == 39 && !(*double_q))
	{
		*single_q = !*single_q;
		return (1);
	}
	return (0);
}

char	*rm_quotes(char *str)
{
	int		i;
	int		j;
	int		single_q;
	int		double_q;
	char	*res;

	i = 0;
	j = 0;
	single_q = 0;
	double_q = 0;
	if (!str)
		return (NULL);
	res = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (q_handle(str[i], &single_q, &double_q))
			i++;
		else
			res[j++] = str[i++];
	}
	if (single_q == 1 || double_q == 1)
		return (free(str), printf("Syntax Error!\n"), NULL);
	res[j] = 0;
	return (res);
}

// char	*expand_str(char *str, t_env *env)
// {
// 	int		i;
// 	int		j;
// 	char	*value;
// 	char	*var;
// 	char	*res;
// 	char	*tmp;
// 	int		in_single;
// 	int		in_double;
// 	t_env	*finded;
// 			char temp[2];

// 	in_single = 0;
// 	in_double = 0;
// 	i = 0;
// 	j = 0;
// 	res = ft_strdup("");
// 	var = NULL;
// 	value = NULL;
// 	while (str[i])
// 	{
// 		if (str[i] == 34 && !in_single)
// 		{
// 			if (!in_double)
// 				in_double = 1;
// 			else
// 				in_double = 0;
// 			i++;
// 		}
// 		else if (str[i] == 39 && !in_double)
// 		{
// 			if (!in_single)
// 				in_single = 1;
// 			else
// 				in_single = 0;
// 			i++;
// 		}
// 		else if (str[i] == '$' && in_single == 0)
// 		{
// 			i++;
// 			if (!str[i])
// 				return (NULL);
// 			if (str[i] && str[i] == '$')
// 			{
// 				tmp = res;
// 				value = ft_itoa(getpid());
// 				res = ft_strjoin(res, value);
// 				free(value);
// 				free(tmp);
// 				i++;
// 			}
// 			else if (str[i] && str[i] == '?')
// 			{
// 				tmp = res;
// 				value = ft_itoa(g_data.last_exit_status);
// 				res = ft_strjoin(res, value);
// 				free(value);
// 				free(tmp);
// 				i++;
// 			}
// 			else
// 			{
// 				j = i;
// 				while (str[i] && isalnum(str[i]))
// 					i++;
// 				value = ft_substr(str, j, i - j);
// 				finded = find_env_node(env, value);
// 				if (!finded)
// 					return (NULL);
// 				var = finded->value;
// 				if (!var)
// 					var = "";
// 				tmp = res;
// 				res = ft_strjoin(res, var);
// 				free(tmp);
// 				free(value);
// 			}
// 		}
// 		else if (str[i] == '$' && in_single == 1)
// 		{
// 			tmp = res;
// 			res = ft_strjoin(res, "$");
// 			free(tmp);
// 			i++;
// 		}
// 		else
// 		{
// 			temp[0] = str[i];
// 			temp[1] = '\0';
// 			tmp = res;
// 			res = ft_strjoin(res, temp);
// 			free(tmp);
// 			i++;
// 		}
// 	}
// 	if (in_double == 1 || in_single == 1)
// 	{
// 		printf("Syntax Error!\n");
// 		return (NULL);
// 	}
// 	return (res);
// }

char	*expand_exit(char *res)
{
	char	*tmp;
	char	*value;

	tmp = res;
	value = ft_itoa(g_last_exit_code);
	if (!value)
	{
		free(tmp);
		return (NULL);
	}
	res = ft_strjoin(tmp, value);
	free(tmp);
	free(value);
	if (!res)
		return (NULL);
	return (res);
}

char	*handle_dollar(char *res, char *str, int *i, t_env *env)
{
	if (str[*i] && str[*i] == '$')
	{
		(*i)++;
		return (expand_dollar(res));
	}
	else if (str[*i] && str[*i] == '?')
	{
		(*i)++;
		return (expand_exit(res));
	}
	else
		return (expand_value(res, str, i, env));
}

char	*expand_str(char *str, t_env *env)
{
	char	*res;

	int (i), (single_q), (double_q);
	i = 0;
	single_q = 0;
	double_q = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (str[i])
	{
		check_q(str[i], &double_q, &single_q);
		if (str[i] == '$' && !single_q)
		{
			i++;
			if (!str[i])
				return (free(res), NULL);
			res = handle_dollar(res, str, &i, env);
		}
		else
			res = append_char(res, str[i++]);
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
