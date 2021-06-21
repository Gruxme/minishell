/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_it.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bagh <ael-bagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 13:54:07 by ael-bagh          #+#    #+#             */
/*   Updated: 2021/06/21 14:38:42 by ael-bagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	dollar_finder(char *str, t_list **quotes)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '$' && (is_between_quotes(i, quotes) != S_QUOTE)
			&& (str[i + 1] != '\0' && str[i + 1] != '\"'
				&& str[i + 1] != '\'' && str[i + 1] != ' '))
			return (i);
		i++;
	}
	return (-1);
}

char	*join_one(char *str, char c)
{
	char	*ret;
	int		i;

	i = -1;
	ret = malloc(sizeof(char) * (ft_strlen(str) + 2));
	while (str[++i])
		ret[i] = str[i];
	ret[i] = c;
	ret[i + 1] = '\0';
	free(str);
	return (ret);
}

char	*magic_touch(char *str)
{
	int		i;
	t_list	*tmp;
	char	*ret_str;

	tmp = NULL;
	ret_str = ft_strdup("");
	i = quotes_finder(str, &tmp);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!is_valid_quote(i, &tmp))
				ret_str = join_one(ret_str, str[i]);
			else
				continue ;
		}
		else
			ret_str = join_one(ret_str, str[i]);
	}
	return (ret_str);
}

char	*expand(char *tab, t_list **envl)
{
	int		tmp[3];
	char	*ret;
	char	*key;
	t_envl	*var;
	t_list	*quotes;

	ret = NULL;
	quotes = NULL;
	tmp[1] = quotes_finder(tab, &quotes);
	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = -1;
	if (dollar_finder(tab, &quotes) == -1)
		return (tab);
	while (tab[++tmp[2]])
	{
		if (tab[tmp[2]] == '$')
		{
			tmp[0] = tmp[2];
			while ((tab[tmp[2]] != ' ' && tab[tmp[2]] != '\"'
					&& tab[tmp[2]] != '\'')
				&& tab[tmp[2]])
				tmp[2]++;
			key = ft_substr(tab, tmp[0] + 1, tmp[2] - (tmp[0] + 1));
			var = find_env_var(key, envl);
			ret = ft_strndup(tab, tmp[0]);
			if (var)
				ret = ft_strjoin(ret, var->value);
			ret = ft_strjoin(ret, &tab[tmp[2]]);
			ft_lstclear(&quotes, del_node);
			tmp[1] = quotes_finder(ret, &quotes);
			if (dollar_finder(ret, &quotes) >= 0)
				expand(ret, envl);
			if (tab[tmp[2]] == '\0')
				return (ret);
		}
	}
	return (ret);
}

char	**spank_it(char **tab, t_list **envl)
{
	int		i;
	char	**ret;
	char	**final;

	i = two_d_counter(tab);
	ret = (char **)malloc((i + 1) * sizeof(char *));
	i = -1;
	while (tab[++i])
		ret[i] = expand(tab[i], envl);
	ret[i] = NULL;
	final = (char **)malloc((i + 1) * sizeof(char *));
	i = -1;
	while (ret[++i])
		final[i] = magic_touch(ret[i]);
	final[i] = NULL;
	return (final);
}
