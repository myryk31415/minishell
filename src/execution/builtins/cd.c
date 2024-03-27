/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 10:14:07 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/27 15:47:48 by antonweizma      ###   ########.fr       */
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
		tmp = ft_calloc(sizeof(char), j);
		ft_strlcpy(tmp, arg + i, j + 1);
		if (!ft_strncmp(tmp, "..", 2))
			path = get_up_dir(path);
		else if (ft_strncmp(tmp, ".", 2))
			path = add_path(path, tmp);
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

int	cd(char	*arg, char ***env)
{
	char	*path_to_dir;

	if (!arg || !ft_strncmp(arg, "~", 2))
		path_to_dir = get_env(*env, "HOME");
	else if (arg[0] == '-')
	{
		path_to_dir = get_env(*env, "OLDPWD");
		if (!path_to_dir)
			return (free(path_to_dir), \
				ft_putstr_fd("bash: cd: OLDPWD not set\n", 2), EXIT_FAILURE);
	}
	else
		path_to_dir = cd_path(arg, *env);
	if (!path_to_dir)
		return (error_msg("bash: cd: ", arg), EXIT_FAILURE);
	if (access(path_to_dir, F_OK | X_OK))
		return (error_msg("bash: cd: ", arg), free(path_to_dir), EXIT_FAILURE);
	system("leaks minishell");
	oldpwd_save(env);
	chdir(path_to_dir);
	if (arg && arg[0] == '-')
		pwd();
	free (path_to_dir);
	return (0);
}
