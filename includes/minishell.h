/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 18:14:34 by abiari            #+#    #+#             */
/*   Updated: 2021/03/05 16:08:02 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct	s_cmds
{
	char			*cmd;
	int				type;
	struct	s_cmds	*prev;
	struct	s_cmds	*next;
}				t_cmds;

int		g_status;
#endif