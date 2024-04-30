/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:21:06 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 16:23:27 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	redirect_nodes(t_redir *token, int **pipes, t_exec *exec)
{
	command(token->redirects, pipes, 1, exec);
	if (token->type == CMD)
		command((t_cmd *)token->next, pipes, 0, exec);
	else if (token->type == OR)
		or_execute((t_node *)token->next, pipes, exec);
	else if (token->type == AND)
		and_execute((t_node *)token->next, pipes, exec);
	else if (token->type == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->next, exec, pipes);
	else if (token->type == REDIR)
		redirect((t_redir *)token->next, pipes, 1, exec);
	free(pipes);
}

int	redirect(t_redir *token, int **pipes, int status, t_exec *exec)
{
	int	pid;
	int	pipe_exit;

	pipe_exit = 0;
	if (status == 1)
		pipe_exit = 1;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid)
	{
		redirect_nodes(token, pipes, exec);
		exit_shell(exec, NULL, exec->exit_status);
	}
	else
	{
		close_pipes(pipes);
		exec->exit_status = new_waitpid(pid);
		if (pipe_exit)
		{
			free(pipes);
			exit_shell(exec, NULL, exec->exit_status);
		}
	}
	return (exec->exit_status);
}
