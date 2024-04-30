/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/30 16:29:07 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	pipe_left(t_exec *tmp, t_node *token, int **pipes, int **redir_pipes)
{
	t_exec	exec;

	exec = *tmp;
	if (!tmp->sub_process)
	{
		free(tmp);
		exec.sub_process = 1;
	}
	if (redir_pipes)
		close_pipe(redir_pipes[0]);
	free(redir_pipes);
	if (token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 0, &exec);
	else if (token->type_left == REDIR)
		redirect((t_redir *)token->left, pipes, 1, &exec);
}


void	pipe_right(t_exec *exec, int **pipes, int **redir_pipes, t_node *token)
{
	t_exec	exec_tmp;

	exec_tmp = *exec;
	if (!exec->sub_process)
	{
		free(exec);
		exec_tmp.sub_process = 1;
	}
	close_pipe(pipes[1]);
	pipes[1] = pipes[0];
	close(pipes[1][1]);
	if (redir_pipes && redir_pipes[0])
		pipes[0] = redir_pipes[0];
	else
		pipes[0] = NULL;
	free(redir_pipes);
	if (token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 0, &exec_tmp);
	else if (token->type_right == REDIR)
		redirect((t_redir *)token->right, pipes, 1, &exec_tmp);
}

void	run_tree(t_node *token, int **pipes, t_exec *exec, int **redir_pipes)
{
	int		id;
	int		*tmp;

	id = 1;
	if (token->type_right != PIPE)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
			pipe_right(exec, pipes, redir_pipes, token);
	}
	if (id && token->type_right == PIPE)
	{
		close_pipe(pipes[1]);
		tmp = pipes[0];
		free(pipes);
		exec->exit_status = create_tree(tmp, \
			(t_node *)token->right, exec, redir_pipes);
		return ;
	}
	close_pipes(pipes);
	exec->exit_status = new_waitpid(id);
	free(pipes);
}

int	create_tree(int *pre_fd, t_node *token, t_exec *tmp, int **redir_pipes)
{
	int	fd[2];
	int	**pipes;
	int	pid;

	pipes = malloc(sizeof(int *) * 2);
	if (!pipes)
		perror("Malloc");
	if (pipe(fd) == -1)
		perror("Pipe");
	pipes[0] = fd;
	pipes[1] = pre_fd;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (redir_pipes && redir_pipes[1])
	{
		close_pipe(pipes[1]);
		pipes[1] = redir_pipes[1];
		redir_pipes[1] = NULL;
	}
	if (!pid)
		pipe_left(tmp, token, pipes, redir_pipes);
	run_tree(token, pipes, tmp, redir_pipes);
	waitpid(pid, NULL, 0);
	return (tmp->exit_status);
}
