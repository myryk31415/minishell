/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/11 23:31:36 by padam            ###   ########.fr       */
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
			free_str_array(cmd_path);
			return (trial_path);
		}
		free(trial_path);
		i++;
	}
	if (!cmd_path)
		free_str_array(cmd_path);
	if (!path_to_cmd)
		free(cmd_path);
	return (NULL);
}

int	is_builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	expander_array(token->args, exec);
	if (!ft_strncmp(token->args[0], "cd", 3) || !ft_strncmp(token->args[0], \
		"echo", 5) || !ft_strncmp(token->args[0], "pwd", 4) || \
		!ft_strncmp(token->args[0], "export", 7) || !ft_strncmp(token->args[0], \
		"unset", 6) || !ft_strncmp(token->args[0], "exit", 5) || \
		!ft_strncmp(token->args[0], "env", 4))
	{
		if (in_and_out_handling(token, pipes, redir) == -1)
			return (EXIT_FAILURE);
		if (!ft_strncmp(token->args[0], "cd", 3))
			return (cd(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "echo", 5))
			return (echo(token->args));
		else if (!ft_strncmp(token->args[0], "pwd", 4))
			return (pwd());
		else if (!ft_strncmp(token->args[0], "export", 7))
			return (export(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "exit", 5))
			exit_shell(exec, token->args, 0);
		else if (!ft_strncmp(token->args[0], "unset", 6))
			return (unset(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "env", 4))
			return (env_cmd(token, *(exec->env)));
	}
	return (-1);
}

void	execute(char **cmd_arg, t_exec *exec)
{
	char		*cmd_path;

	expander_array(cmd_arg, exec);
	if (!access(cmd_arg[0], F_OK | X_OK))
	{
		node_tree_delete(exec->tree, exec->type);
		// rl_clear_history();
		execve(cmd_arg[0], cmd_arg, *(exec->env));
		ft_putstr_fd(ft_strjoin("minishell: ", \
			ft_strjoin(cmd_arg[0], ": command not found\n")), 2);
		free_env(exec->env);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (!ft_strchr(cmd_arg[0], '/'))
			cmd_path = get_path(cmd_arg[0], *(exec->env), "PATH");
		node_tree_delete(exec->tree, exec->type);
		// rl_clear_history();
		execve(cmd_path, cmd_arg, *(exec->env));
		ft_putstr_fd(ft_strjoin("minishell: ", \
			ft_strjoin(cmd_arg[0], ": command not found\n")), 2);
		free_env(exec->env);
		exit(EXIT_FAILURE);
	}
}
