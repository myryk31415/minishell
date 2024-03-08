/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/07 11:42:57 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_env(char **envp, char *var)
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
		if (!ft_strncmp(tmp, var, ft_strlen(var))
			&& !ft_strncmp(tmp, var, ft_strlen(tmp)))
		{
			free(tmp);
			return (envp[i] + j + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp, char *var)
{
	char	**cmd_path;
	char	*path_to_cmd;
	char	*trial_path;
	int		i;

	cmd_path = ft_split(get_env(envp, var), ':');
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

void	exec(char **cmd_arg)
{
	char		*cmd_path;
	extern char	**environ;

	cmd_path = get_path(cmd_arg[0], environ, "PATH");
	execve(cmd_path, cmd_arg, environ);
	perror("Command not found\n");
	exit(-1);
}

int	here_doc(char *limiter)
{
	char	*str;
	int		fd[2];

	if (pipe(fd) == -1)
		perror("Pipe");
	while (1)
	{
		str = readline(">");
		if (str && *str)
		{
			if (!ft_strncmp(str, limiter, ft_strlen(limiter))
				&& !ft_strncmp(str, limiter, ft_strlen(str) - 1))
			{
				free(str);
				break ;
			}
			write(fd[1], str, ft_strlen(str));
			free(str);
		}
	}
	close(fd[1]);
	return (fd[0]);
}

void	free_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}
