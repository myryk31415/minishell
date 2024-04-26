/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/26 11:20:38 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	create_tree(int *pre_fd, t_node *token, t_exec exec, int **redir_pipes)
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
	{
		if (redir_pipes)
			close_pipe(redir_pipes[0]);
		if (token->type_left == CMD)
			command((t_cmd *)token->left, pipes, 0, &exec);
		else if (token->type_left == REDIR)
			redirect((t_redir *)token->left, pipes, 1, exec);
	}
	run_tree(token, pipes, &exec, redir_pipes);
	waitpid(pid, NULL, 0);
	free(pipes);
	return (exec.exit_status);
}

void	run_tree(t_node *token, int **pipes, t_exec *exec, int **redir_pipes)
{
	int	status;
	int	id;

	id = 1;
	if (token->type_right != PIPE)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			close_pipe(pipes[1]);
			pipes[1] = pipes[0];
			if (redir_pipes && redir_pipes[0])
				pipes[0] = redir_pipes[0];
			else
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
		exec->exit_status = create_tree(pipes[0], \
			(t_node *)token->right, *exec, redir_pipes);
		return ;
	}
	close_pipes(pipes);
	close_pipes(redir_pipes);
	waitpid(id, &status, 0);
	if (token->type_right != PIPE && WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	// ft_putstr_fd("REDIR: exit status: ", 2);
	// ft_putnbr_fd(exec->exit_status, 2);
	// ft_putstr_fd("\n", 2);
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
		exec->exit_status = create_tree(0, (t_node *)tree, *exec, NULL);
	else if (type == REDIR)
		exec->exit_status = redirect((t_redir *)tree, NULL, 0, *exec);
	if (exec->exit_status == 256)
		exec->exit_status = 1;
	// 	ft_putstr_fd("REDIR: exit status: ", 2);
	// ft_putnbr_fd(exec->exit_status, 2);
	// ft_putstr_fd("\n", 2);
	return ;
}
