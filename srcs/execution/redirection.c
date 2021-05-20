/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:10:26 by abiari            #+#    #+#             */
/*   Updated: 2021/05/20 10:38:13 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect(t_pipeline *cmd)
{
	t_redirect	*files;
	int			fd;

	files = cmd->redirections;

	while (files)
	{
		if (files->type == APP_R)
			fd = open(files->file, O_CREAT | O_APPEND | O_WRONLY, S_IRUSR |
			S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		else if (files->type == OUT_R)
			fd = open(files->file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR |
			S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		else
			fd = open(files->file, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			exit(errno);
		}
		files = files->next;
	}
	if (files->type == IN_R)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (files->type == OUT_R)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
