/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 10:14:07 by aweizman          #+#    #+#             */
/*   Updated: 2024/05/01 12:43:53 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_up_dir(char *path)
{
	char	*new_path;
	int		i;

	i = ft_strlen(path) - 1;
	while (path[i] && path[i] != '/')
		i--;
	i--;
	new_path = ft_calloc(sizeof(char), i + 2);
	ft_strlcpy(new_path, path, i + 2);
	free(path);
	return (new_path);
}

char	*add_path(char *path, char *tmp)
{
	char	*new_path;
	char	*tmp2;

	if (tmp[0] != '/')
	{
		tmp2 = ft_strjoin(path, "/");
		free(path);
	}
	else
		tmp2 = path;
	new_path = ft_strjoin(tmp2, tmp);
	free(tmp2);
	free(tmp);
	return (new_path);
}

char	*cd_copy_path(char *path, char *arg, int i, int j)
{
	char	*tmp;

	while (arg[i])
	{
		j = 1;
		while (arg[i + j] && arg[i + j] != '/')
			j++;
		tmp = ft_calloc(sizeof(char), j + 1);
		ft_strlcpy(tmp, arg + i, j + 1);
		if (!ft_strncmp(tmp, "..", 2))
		{
			path = get_up_dir(path);
			free(tmp);
		}
		else if (ft_strncmp(tmp, ".", 2))
			path = add_path(path, tmp);
		else
			free(tmp);
		arg = arg + j;
	}
	return (path);
}

char	*cd_path(char *arg, char **env)
{
	char	*path;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (arg[0] == '/')
		path = ft_calloc(1, 1);
	else if (arg[0] == '~')
	{
		path = get_env(env, "HOME");
		i = 2;
	}
	else
		path = getcwd(NULL, PATH_MAX);
	path = cd_copy_path(path, arg, i, j);
	return (path);
}

int	cd(char	**arg, char ***env)
{
	char	*path;

	if (!*arg || !ft_strncmp(*arg, "~", 2))
	{
		path = get_env(*env, "HOME");
		if (!path)
			return (ft_putstr_fd("minishell: cd: \
HOME not set\n", 2), EXIT_FAILURE);
	}
	else if ((*arg)[0] == '-')
	{
		path = get_env(*env, "OLDPWD");
		if (!path)
			return (ft_putstr_fd("minishell: cd: \
OLDPWD not set\n", 2), EXIT_FAILURE);
	}
	else
		path = cd_path(*arg, *env);
	if (!path || access(path, F_OK | X_OK))
		return (error_msg("minishell: cd: ", *arg), free(path), EXIT_FAILURE);
	oldpwd_save(env, path, *arg);
	return (0);
}
