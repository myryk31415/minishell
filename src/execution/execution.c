/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/09 10:56:58 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	create_tree(int *pre_fd, t_node *token, t_exec exec)
{
	int	fd[2];
	int	**pipes;
	int	pid;
	int	id;

	id = 1;
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
	if (!pid && token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 0, &exec);
	else if (!pid && token->type_left == REDIR)
		redirect((t_redir *)token->left, pipes, 1, exec);
	run_tree(token, pipes, &exec, id);
	waitpid(pid, NULL, 0);
	free(pipes);
	return (exec.exit_status);
}

void	run_tree(t_node *token, int **pipes, t_exec *exec, int id)
{
	if (token->type_right != PIPE)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			close_pipe(pipes[1]);
			pipes[1] = pipes[0];
			pipes[0] = NULL;
			if (token->type_right == CMD)
				command((t_cmd *)token->right, pipes, 0, exec);
			else if (token->type_right == REDIR)
				redirect((t_redir *)token->right, pipes, 1, *exec);
		}
	}
	if (id && token->type_right == PIPE)
	{
		close_pipe(pipes[1]);
		create_tree(pipes[0], (t_node *)token->right, *exec);
		close_pipes(pipes);
	}
	close_pipes(pipes);
	waitpid(id, &(exec->exit_status), 0);
}

void	execution(void *tree, t_node_type type, t_exec *exec)
{

	if (type == CMD)
		command((t_cmd *)tree, NULL, 2, exec);
	else if (type == AND)
		and_execute((t_node *)tree, 0, NULL, exec);
	else if (type == OR)
		or_execute((t_node *)tree, 0, NULL, exec);
	else if (type == PIPE)
		exec->exit_status = create_tree(0, (t_node *)tree, *exec);
	else if (type == REDIR)
		exec->exit_status = redirect((t_redir *)tree, NULL, 0, *exec);
	if (exec->exit_status == 256)
		exec->exit_status = 1;
	return ;
}
