/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bagh <ael-bagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 12:18:09 by ael-bagh          #+#    #+#             */
/*   Updated: 2021/04/22 15:44:37 by ael-bagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	len_init(int index, int *array, int last, char *cmd)
{
	if (index == 0)
		return (array[index]);
	else if (index == last && check_last_cmd(cmd, array[last - 1]))
		return (ft_strlen(cmd) - array[index - 1] - 1);
	else
		return (array[index] - array[index - 1] - 1);
}

int	i_init(int index, int *array, int last, char *cmd)
{
	if (index == 0)
		return (0);
	else if (index == last && check_last_cmd(cmd, array[last - 1]))
		return (array[index - 1] + 1);
	else
		return (array[index - 1] + 1);
}

int	check_cmds_helper(char **cmds, char *cmd, t_list *tmp, int *comma)
{
	int	last;
	int	i;

	i = -1;
	last = last_char(comma);
	printf("%d\n", cmd_counter(comma, cmd) - 1);
	while (cmds[++i])
	{
		if (i == cmd_counter(comma, cmd) - 1)
			if (only_char(' ', cmds[i])
				&& (comma[last - 1] == (int)ft_strlen(cmd) - 1))
				free_the_nipples(tmp, cmds, cmd_counter(comma, cmd));
		if (i != cmd_counter(comma, cmd) - 1 && only_char(' ', cmds[i]))
			free_the_nipples(tmp, cmds, cmd_counter(comma, cmd));
	}
	if (comma)
		free(comma);
	i = 0;
	while (cmds[i])
	{
		printf("%s\n", cmds[i]);
		i++;
	}
	ft_free(cmds, i);
	ft_lstclear(&tmp, del_node);
	return (0);
}

int	check_cmds(char **cmds, char *cmd)
{
	int		i;
	int		*comma;
	t_list	*tmp;
	int		ret;

	tmp = NULL;
	ret = quotes_finder(cmd, &tmp);
	comma = commas(cmd, &tmp);
	if (comma != NULL)
		return (check_cmds_helper(cmds, cmd, tmp, comma));
	i = 0;
	while (cmds[i])
	{
		printf("%s\n", cmds[i]);
		i++;
	}
	ft_free(cmds, i);
	return (0);
}