/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:23:50 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 16:38:46 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	token->args = expander_array(token->args, exec);
	if (*(token->args) && (!ft_strncmp(token->args[0], "cd", 3) || \
	!ft_strncmp(token->args[0], "echo", 5) || !ft_strncmp(token->args[0], "pwd"\
	, 4) || !ft_strncmp(token->args[0], "export", 7) || \
	!ft_strncmp(token->args[0], "unset", 6) || !ft_strncmp(token->args[0], \
	"exit", 5) || !ft_strncmp(token->args[0], "env", 4)))
	{
		if (in_and_out_hdl_fork(token, pipes, redir, exec) == 1)
			return (EXIT_FAILURE);
		if (!ft_strncmp(token->args[0], "cd", 3))
			return (cd((token->args + 1), exec->env));
		else if (!ft_strncmp(token->args[0], "echo", 5))
			return (echo(token->args));
		else if (!ft_strncmp(token->args[0], "pwd", 4))
			return (pwd());
		else if (!ft_strncmp(token->args[0], "export", 7))
			return (export(token->args, exec->env, 0));
		else if (!ft_strncmp(token->args[0], "exit", 5))
			return (exit_shell(exec, token->args, -1));
		else if (!ft_strncmp(token->args[0], "unset", 6))
			return (unset(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "env", 4))
			return (env_cmd(token, *(exec->env)));
	}
	return (-1);
}

int	is_builtin_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	token->args = expander_array(token->args, exec);
	if (*(token->args) && (!ft_strncmp(token->args[0], "cd", 3) || \
	!ft_strncmp(token->args[0], "echo", 5) || !ft_strncmp(token->args[0], "pwd"\
	, 4) || !ft_strncmp(token->args[0], "export", 7) || \
	!ft_strncmp(token->args[0], "unset", 6) || !ft_strncmp(token->args[0], \
	"exit", 5) || !ft_strncmp(token->args[0], "env", 4)))
	{
		if (in_and_out_hdl_no_fork(token, pipes, redir, exec) == 1)
			return (EXIT_FAILURE);
		if (!ft_strncmp(token->args[0], "cd", 3))
			return (cd((token->args + 1), exec->env));
		else if (!ft_strncmp(token->args[0], "echo", 5))
			return (echo(token->args));
		else if (!ft_strncmp(token->args[0], "pwd", 4))
			return (pwd());
		else if (!ft_strncmp(token->args[0], "export", 7))
			return (export(token->args, exec->env, 0));
		else if (!ft_strncmp(token->args[0], "exit", 5))
			return (exit_shell(exec, token->args, -1));
		else if (!ft_strncmp(token->args[0], "unset", 6))
			return (unset(token->args, exec->env));
		else if (!ft_strncmp(token->args[0], "env", 4))
			return (env_cmd(token, *(exec->env)));
	}
	return (-1);
}

int	builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int		old_stdin;
	int		old_stdout;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	exec->exit_status = is_builtin(token, pipes, redir, exec);
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
	if (exec->exit_status == -1)
	{
		exec->exit_status = 0;
		return (-1);
	}
	return (exec->exit_status);
}
