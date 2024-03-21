/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/21 12:06:40 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_env(char **environ, char *var)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (environ[i])
	{
		j = 0;
		while (environ[i][j] && environ[i][j] != '=')
			j++;
		tmp = ft_substr(environ[i], 0, j);
		if (!tmp)
			break ;
		if (!ft_strncmp(tmp, var, ft_strlen(var))
			&& !ft_strncmp(tmp, var, ft_strlen(tmp)))
		{
			free(tmp);
			return (environ[i] + j + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **environ, char *var)
{
	char	**cmd_path;
	char	*path_to_cmd;
	char	*trial_path;
	int		i;

	cmd_path = ft_split(get_env(environ, var), ':');
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

int	is_builtin(t_cmd *token, int *fd, int *pre_fd, int *redir)
{
	if (!ft_strncmp(token->args[0], "cd", 2))
	{
		command(token, fd, pre_fd, redir);
		return (cd(token->args[1]));
	}
	if (!ft_strncmp(token->args[0], "echo", 4))
	{
		command(token, fd, pre_fd, redir);
		return (echo(token->args));
	}
	if (!ft_strncmp(token->args[0], "pwd", 3))
	{
		command(token, fd, pre_fd, redir);
		return (pwd());
	}
	return (1);
}

void	exec(char **cmd_arg)
{
	char		*cmd_path;
	extern char	**environ;

	if (!access(cmd_arg[0], F_OK | X_OK))
	{
		execve(cmd_arg[0], cmd_arg, environ);
		perror("Command not found");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (!ft_strchr(cmd_arg[0], '/'))
			cmd_path = get_path(cmd_arg[0], environ, "PATH");
		execve(cmd_path, cmd_arg, environ);
		perror("Command not found");
		exit(EXIT_FAILURE);
	}
}

void	free_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}
