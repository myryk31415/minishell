/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/26 12:49:17 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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


int	is_builtin(t_cmd *token, int **pipes, int *redir, char ***env)
{
	if (!ft_strncmp(token->args[0], "cd", 3) || !ft_strncmp(token->args[0], \
		"echo", 5) || !ft_strncmp(token->args[0], "pwd", 4) || \
		!ft_strncmp(token->args[0], "export", 7) || !ft_strncmp(token->args[0] ,\
		"unset", 6) || !ft_strncmp(token->args[0], "exit", 5))
	{
		command(token, pipes, redir);
		if (!ft_strncmp(token->args[0], "cd", 3))
			return (cd(token->args[1], env));
		else if (!ft_strncmp(token->args[0], "echo", 5))
			return (echo(token->args));
		else if (!ft_strncmp(token->args[0], "pwd", 4))
			return (pwd());
		else if (!ft_strncmp(token->args[0], "export", 7))
			return (export(token->args, env));
		// else if (!ft_strncmp(token->args[0], "exit", 5))
		// 	return (exit_shell(token->args));
		else if (!ft_strncmp(token->args[0], "unset", 6))
			return (unset(token->args, env));
	}
	return (1);
}

void	exec(char **cmd_arg, char **env)
{
	char		*cmd_path;

	if (!access(cmd_arg[0], F_OK | X_OK))
	{
		execve(cmd_arg[0], cmd_arg, env);
		ft_putstr_fd(ft_strjoin("minishell: ", \
			ft_strjoin(cmd_arg[0], ": command not found\n")), 2);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (!ft_strchr(cmd_arg[0], '/'))
			cmd_path = get_path(cmd_arg[0], env, "PATH");
		execve(cmd_path, cmd_arg, env);
		ft_putstr_fd(ft_strjoin("minishell: ", \
			ft_strjoin(cmd_arg[0], ": command not found\n")), 2);
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
