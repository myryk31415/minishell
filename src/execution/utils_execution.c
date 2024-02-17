/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/17 16:16:28 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
		if (ft_strncmp(tmp, "PATH", 4) == 0)
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

void	exec(char *cmd)
{
	char		*cmd_path;
	char		**cmd_arg;
	extern char	**environ;

	cmd_arg = ft_split(cmd, ' ');
	cmd_path = get_path(cmd_arg[0], environ);
	if (execve(cmd_path, cmd_arg, environ) == -1)
		perror("Command not found\n");
}

void	here_doc(t_execution *input)
{
	char	*str;

	while (1)
	{
		str = get_next_line(STDIN_FILENO);
		if (str && *str)
		{
			if (!ft_strncmp(str, args->argv[2], ft_strlen(args->argv[2]))
				&& !ft_strncmp(str, args->argv[2], ft_strlen(str) - 1))
			{
				free(str);
				break ;
			}
			write(args->here_doc_pipe[1], str, ft_strlen(str));
			free(str);
		}
	}
	close(args->here_doc_pipe[1]);
}
