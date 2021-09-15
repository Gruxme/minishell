/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bagh <ael-bagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/28 13:37:19 by ael-bagh          #+#    #+#             */
/*   Updated: 2021/09/15 09:26:47 by ael-bagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_freex(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
	tab = NULL;
}

char	**parse_line(char *line)
{
	char	**tab;

	tab = split_cmds(line);
	if (tab)
		if (check_cmds(tab, line) == 1)
			return (NULL);
	return (tab);
}

int	check_pipes(char **pipe, char *cmd)
{
	int		*pipe_arr;
	t_list	*tmp;

	if (pipe == NULL)
		return(1);
	tmp = NULL;
	quotes_finder(cmd, &tmp);
	pipe_arr = pipes(cmd, &tmp);
	if (pipe_arr != NULL)
		return (check_pipes_helper(pipe, cmd, tmp, pipe_arr));
	ft_lstclear(&tmp, del_node);
	return (0);
}

void lst_append_red(t_redirect **list, t_redirect *param)
{
	if ((*list))
		lst_append_red(&(*list)->next, param);
	else
		(*list) = param;
}

t_redirect	*red_lst(char **red, char *cmd, t_pipeline *pipe_lst, t_list **envl)
{
	t_redirect	*red_list;
	t_redirect	*head;
	t_list		*quotes;
	t_list		*reds;
	int			v;
	(void)envl;
	reds = NULL;
	quotes = NULL;
	pipe_lst->redirections = NULL;
	v = quotes_finder(cmd, &quotes);
	v = red_finder(cmd, &reds, &quotes);
	v = 1;
	red_list = malloc(sizeof(t_redirect));
	red_list->file = ft_strdup(red[v]);
	red_list->type = red_type(&reds, v);
	red_list->next = NULL;
	lst_append_red(&(pipe_lst->redirections), red_list);
	head = red_list;
	while (red[++v])
	{
		red_list = malloc(sizeof(t_redirect));
		red_list->file = ft_strdup(red[v]);
		red_list->type = red_type(&reds, v);
		red_list->next = NULL;
		lst_append_red(&(pipe_lst->redirections), red_list);
	}
	ft_lstclear(&reds, del_node_r);
	return (head);
}

void lst_append_pipe(t_pipeline **list, t_pipeline *param) {
	if ((*list))
		lst_append_pipe(&(*list)->next, param);
	else
		(*list) = param;
}

t_pipeline	*pipe_lst(char **pipelist, t_cmd *cmd_list, t_list **envl)
{
	t_pipeline	*pipe_list;
	int			v;
	char **red;

	v = -1;
	cmd_list->pipes = NULL;
	while (pipelist[++v])
	{
		pipe_list = malloc(sizeof(t_pipeline));
		pipe_list->has_red = 0;
		pipe_list->redirections = NULL;
		red = reddit(pipelist[v]);
		if (red)
		{
			pipe_list->has_red = 1;
			pipe_list->redirections = red_lst(red, pipelist[v], pipe_list, envl);
			pipe_list->cmd	= space_it(red[0]);
		}
		else
		{
			pipe_list->cmd	= space_it(pipelist[v]);
			pipe_list->redirections = NULL;
		}
		if (red)
			free_chard(red);
		pipe_list->next = NULL;
		lst_append_pipe(&(cmd_list->pipes), pipe_list);
	}
	return (cmd_list->pipes);
}

t_cmd	*cmd_lst(char **pipelist, t_list **envl)
{
	t_cmd *cmd_list;
	cmd_list = malloc(sizeof(t_cmd));
	cmd_list->pipes = pipe_lst(pipelist, cmd_list, envl);
	return(cmd_list);
}

int		check_reds(char **pipe)
{
	int	i;
	int	j;
	char **red;
	
	i = -1;
	while (pipe[++i])
	{
		red = reddit(pipe[i]);
		j = 0;
		if (red)
		{
			while (red[++j])
				if (only_char(' ', red[j]))
				{
					free_chard(red);
					ft_putstr_fd("bash: syntax error near unexpected token `>'\n", 2);
					return (0);
				}
			free_chard(red);
		}
	}
	return (1);
}

t_list		*main_lst(char *cmd, t_list **envl)
{
	t_list	*tmp;
	t_list	*tp;
	char	**pipe;
	char	*exp;
	t_cmd	*cmd_list;
	t_list	*main_list;
	int		ret;

	ret = 0;
	exp = NULL;
	tmp = NULL;
	tp = NULL;
	main_list = NULL;
	pipe = NULL;
	exp = expand(cmd, envl);
	ret = quotes_finder(exp, &tmp);
	if (ret == -1)
	{
		ft_putstr_fd("Quotes unclosed\n", 2);
		ft_lstclear(&tmp, del_node);
		//ft_lstclear(&tp, del_node);
		free(exp);
		return (NULL);
	}
	ret = red_finder(exp, &tp, &tmp);
	if (ret == -1)
	{
		ft_lstclear(&tmp, del_node);
		ft_lstclear(&tp, del_node_r);
		free(exp);
		return (NULL);
	}
	pipe = pipe_it(exp);
	if (check_pipes(pipe, exp) == 1 || check_reds(pipe) == 0)
	{
		ft_lstclear(&tmp, del_node);
		ft_lstclear(&tp, del_node_r);
		if (pipe)
			free_chard(pipe);
		free(exp);
		return (NULL);
	}
	cmd_list = cmd_lst(pipe, envl);
	if (pipe)
		free_chard(pipe);
	if (!cmd_list && ret >= 1)
	{
		ft_lstclear(&tmp, del_node);
		ft_lstclear(&tp, del_node_r);
		free(exp);
		return (NULL);
	}
	lst_append(&main_list, cmd_list);
	ft_lstclear(&tmp, del_node);
	ft_lstclear(&tp, del_node_r);
	free(exp);
	return (main_list);
}
