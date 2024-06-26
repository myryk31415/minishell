/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:38:37 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 15:17:04 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command_fork(t_cmd *token, t_exec *exec, int **pipes, int *redir)
{
	int			id;
	char		**tmp;

	if (builtin(token, pipes, redir, exec) == -1)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			if (in_and_out_hdl_fork(token, pipes, redir, exec) == 1)
				exit_shell(exec, NULL, EXIT_FAILURE);
			tmp = token->args;
			token->args = NULL;
			free(pipes);
			execute(tmp, exec, exec->sub_process);
		}
		else
			exec->exit_status = new_waitpid(id);
	}
}

void	command_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	char		**tmp;

	exec->exit_status = is_builtin_no_fork(token, pipes, redir, exec);
	if (exec->exit_status == -1)
	{
		exec->exit_status = 0;
		if (in_and_out_hdl_no_fork(token, pipes, redir, exec) == 1)
			exit_shell(exec, NULL, EXIT_FAILURE);
		tmp = token->args;
		token->args = NULL;
		execute(tmp, exec, exec->sub_process);
	}
	exit_shell(exec, NULL, exec->exit_status);
}

void	command(t_cmd *token, int **pipes, int redirect, t_exec *exec)
{
	static int	redir[2];

	if (redirect == 1)
	{
		if (token->redirects && *(token->redirects))
			handle_both(redir, token, exec);
		return ;
	}
	else
	{
		if (redirect == 0)
			command_no_fork(token, pipes, redir, exec);
		else
			command_fork(token, exec, pipes, redir);
	}
}
