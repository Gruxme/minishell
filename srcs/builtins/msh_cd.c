/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiari <abiari@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 16:08:37 by abiari            #+#    #+#             */
/*   Updated: 2021/06/07 14:44:06 by abiari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	msh_cd(char **args, t_list *envl)
{
	int		ret;
	char	*old_path;
	char	*path;
	char	*home;

	path = args[0];
	old_path = NULL;
	ret = 0;
	old_path = getcwd(NULL, 0);
	if (old_path == NULL)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		return ((int)errno);
	}
	if (path == NULL || (ft_strcmp(path, "~") == 0))
	{
		home = find_env_var("HOME", &envl)->value;
		if (home != NULL)
			ret = chdir(home);
		else
		{
			ft_putstr_fd("msh: cd: HOME not set", 2);
			g_vars.exit_code = 1;
			return (g_vars.exit_code);
		}
		g_vars.exit_code = 0;
		return (0);
	}
	ret = chdir(path);
	if (ret != -1)
	{
		path = getcwd(NULL, 0);
		mod_env_var("PWD", path, &envl);
		if (find_env_var("OLDPWD", &envl) != NULL)
			add_env_var("OLDPWD", old_path, &envl);
		else
			mod_env_var("OLDPWD", old_path, &envl);
	}
	else
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		free(old_path);
		return ((int)errno);
	}
	return (ret);
}
