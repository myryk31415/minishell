/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/30 16:27:00 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*try_path(char *trial_path, char **cmd_path, char *path_to_cmd, int i)
{
	while (cmd_path && path_to_cmd && cmd_path[i])
	{
		trial_path = ft_strjoin(cmd_path[i], path_to_cmd);
		if (access(trial_path, F_OK | X_OK) == 0)
			return (trial_path);
		free(trial_path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **environ, char *var)
{
	char	**cmd_path;
	char	*path_to_cmd;
	char	*trial_path;
	char	*tmp;

	trial_path = NULL;
	tmp = get_env(environ, var);
	if (!tmp)
	{
		cmd_path = ft_calloc(2, sizeof(char *));
		cmd_path[1] = NULL;
		cmd_path[0] = ft_strdup("/bin");
	}
	else
		cmd_path = ft_split(tmp, ':');
	free(tmp);
	if (!*cmd)
		return (NULL);
	path_to_cmd = ft_strjoin("/", cmd);
	trial_path = try_path(trial_path, cmd_path, path_to_cmd, 0);
	free_str_array(cmd_path);
	free(path_to_cmd);
	return (trial_path);
}

void	execute_without_path(char **cmd_arg, t_exec *exec, \
	int no_exec, char *cmd_path)
{
	int	exit_status;

	if (!*cmd_arg)
		exit_shell(exec, NULL, EXIT_SUCCESS);
	cmd_path = get_path(*cmd_arg, *(exec->env), "PATH");
	if (!cmd_path)
		cmd_path = *cmd_arg;
	node_tree_delete(exec->tree, exec->type);
	if (ft_strchr(cmd_path, '/') && **cmd_arg != '.')
		execve(cmd_path, cmd_arg, *(exec->env));
	exit_status = error_message(cmd_path);
	free_env(exec->env);
	free_str_array(cmd_arg);
	if (!no_exec)
		free(exec);
	exit(exit_status);
}

void	execute(char **cmd_arg, t_exec *exec, int no_exec)
{
	char		*cmd_path;
	int			exit_status;

	cmd_path = NULL;
	if (*cmd_arg == NULL)
		exit_shell(exec, NULL, EXIT_SUCCESS);
	if (ft_strchr(*cmd_arg, '/') || !ft_strncmp(*cmd_arg, "~", 2) \
		|| !ft_strncmp(*cmd_arg, ".", 2) || !ft_strncmp(*cmd_arg, "..", 3))
	{
		node_tree_delete(exec->tree, exec->type);
		if (!access(*cmd_arg, F_OK | X_OK))
			execve(*cmd_arg, cmd_arg, *(exec->env));
		exit_status = error_message(*cmd_arg);
		free_env(exec->env);
		free_str_array(cmd_arg);
		if (!no_exec)
			free(exec);
		exit(exit_status);
	}
	else
		execute_without_path(cmd_arg, exec, no_exec, cmd_path);
}
