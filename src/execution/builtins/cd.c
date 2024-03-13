/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 10:14:07 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/13 17:06:30 by antonweizma      ###   ########.fr       */
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

	new_path = ft_strjoin(path, tmp);
	free(tmp);
	free(path);
	return (new_path);
}

char	*cd_path(char *arg)
{
	char	*path;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (arg[0] == '/')
		path = malloc(1);
	else
		path = ft_strjoin(getcwd(NULL, PATH_MAX), "/");
	while (arg[i])
	{
		j = 1;
		while (arg[i + j] && arg[i + j] != '/')
			j++;
		tmp = ft_calloc (sizeof(char), j);
		ft_strlcpy(tmp, arg + i, j + 1);
		if (!ft_strncmp(tmp, "..", 2))
			path = get_up_dir(path);
		else if (ft_strncmp(tmp, ".", 1))
			path = add_path(path, tmp);
		arg = arg + j;
	}
	return (path);
}

int	cd(char	*arg)
{
	static char	*prev_dir = NULL;
	char		*path_to_dir;
	extern char	**environ;

	if (!arg)
		path_to_dir = get_env(environ, "HOME");
	else if (arg[0] == '-')
	{
		if (prev_dir == NULL)
			return (ft_putstr_fd("bash: cd: OLDPWD not set\n", 2), 256);
		else
			path_to_dir = prev_dir;
	}
	else
		path_to_dir = cd_path(arg);
	if (!path_to_dir)
		return (perror(error_msg("bash: cd: ", arg)), -1);
	if (access(path_to_dir, F_OK | X_OK))
		return (perror(error_msg("bash: cd: ", arg)), free (path_to_dir), 256);
	prev_dir = getcwd(NULL, PATH_MAX);
	chdir(path_to_dir);
	if (arg && arg[0] == '-')
		pwd();
	free (path_to_dir);
	return (0);
}
