/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:19:40 by antonweizma       #+#    #+#             */
/*   Updated: 2024/02/14 13:57:55 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env(char **envp)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		tmp = ft_substr(envp[i], 0, j);
		if (!tmp)
			break ;
		if (ft_strcmp(tmp, "PATH") == 0)
		{
			free(tmp);
			return (envp[i] + j + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	**cmd_path;
	char	*path_to_cmd;
	char	*trial_path;
	int		i;

	cmd_path = ft_split(get_env(envp), ':');
	path_to_cmd = ft_strjoin("/", cmd);
	i = 0;
	while (cmd_path && path_to_cmd && cmd_path[i])
	{
		trial_path = ft_strjoin(cmd_path[i], path_to_cmd);
		if (access(trial_path, F_OK | X_OK) == 0)
		{
			free(path_to_cmd);
			free_array(cmd_path);
			return (trial_path);
		}
		free(trial_path);
		i++;
	}
	if (!cmd_path)
		free_array(cmd_path);
	if (!path_to_cmd)
		free(cmd_path);
	return (NULL);
}

void	error_msg(char *err, t_args *args)
{
	perror(err);
	free(args);
	exit(1);
}

int	msg(char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}
