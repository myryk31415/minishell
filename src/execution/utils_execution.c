/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:30:12 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/25 01:35:46 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_path(char *cmd, char **environ, char *var)
{
	char	**cmd_path;
	char	*path_to_cmd;
	char	*trial_path;
	int		i;
	char 	*tmp;

	tmp = get_env(environ, var);
	if (!tmp)
	{
		cmd_path = ft_calloc(2, sizeof(char *));
		cmd_path[1] = NULL;
		cmd_path[0] = ft_strdup("/bin");
	}
	cmd_path = ft_split(tmp, ':');
	free(tmp);
	if (!*cmd)
		return (NULL);
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
	token->args = expander_array(token->args, exec);
	if (*(token->args) && (!ft_strncmp(token->args[0], "cd", 3) || !ft_strncmp\
	(token->args[0],"echo", 5) || !ft_strncmp(token->args[0], "pwd", 4) || \
		!ft_strncmp(token->args[0], "export", 7) || !ft_strncmp(token->args[0], \
		"unset", 6) || !ft_strncmp(token->args[0], "exit", 5) || \
		!ft_strncmp(token->args[0], "env", 4)))
	{
		if (in_and_out_handling(token, pipes, redir, exec) == 1)
			return (EXIT_FAILURE);
		if (!ft_strncmp(token->args[0], "cd", 3))
			return (cd(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "echo", 5))
			return (echo(token->args));
		else if (!ft_strncmp(token->args[0], "pwd", 4))
			return (pwd());
		else if (!ft_strncmp(token->args[0], "export", 7))
			return (export(token->args, exec->env, 0));
		else if (!ft_strncmp(token->args[0], "exit", 5))
			exit_shell(exec, token->args, 0);
		else if (!ft_strncmp(token->args[0], "unset", 6))
			return (unset(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "env", 4))
			return (env_cmd(token, *(exec->env)));
	}
	return (-1);
}

int	error_message(char *cmd_path)
{
	DIR *folder;
	int	file;
	int	exit_status;

	file = 0;
	exit_status = 0;
	folder = NULL;
	if (!ft_strncmp(cmd_path, ".", 2))
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		exit_status = 2;
	}
	else if (ft_strchr(cmd_path, '/') == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putendl_fd(": command not found", 2);
		exit_status = 127;
	}
	else
	{
		folder = opendir(cmd_path);
		file = open(cmd_path, O_RDWR);
		if (file == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd_path);
			exit_status = 126;
			if (!folder && errno != EACCES)
				exit_status = 127;
		}
		else if(access(cmd_path, X_OK))
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd_path);
			exit_status = 126;
		}
	}
	if (file)
		close(file);
	return (exit_status);
}

char	*expand_tilde(char *cmd_arg, char **env)
{
	char	*home;
	char	*tmp;

	if (cmd_arg[0] == '~')
	{
		home = get_env(env, "HOME");
		tmp = ft_strjoin(home, cmd_arg + 1);
		free(home);
		free(cmd_arg);
		return (tmp);
	}
	return (cmd_arg);
}

void	execute(char **cmd_arg, t_exec *exec)
{
	char		*cmd_path;
	int			exit_status;

	if (*cmd_arg == NULL)
		exit(EXIT_SUCCESS);
	if (ft_strchr(*cmd_arg, '/') || !ft_strncmp(*cmd_arg, "~", 2) \
		|| !ft_strncmp(*cmd_arg, ".", 2) || !ft_strncmp(*cmd_arg, "..", 3))
	{
		*cmd_arg = expand_tilde(*cmd_arg, *(exec->env));
		node_tree_delete(exec->tree, exec->type);
		if (!access(*cmd_arg, F_OK | X_OK))
			execve(*cmd_arg, cmd_arg, *(exec->env));
		exit_status = error_message(*cmd_arg);
		free_env(exec->env);
		exit(exit_status);
	}
	else
	{
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
		// free(exec);
		exit(exit_status);
	}
}
