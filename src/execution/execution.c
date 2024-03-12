/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/12 15:35:48 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command(t_cmd *token, int *fd, int *pre_fd, int *redir)
{
	int	input;
	int	output;

	input = 0;
	output = 0;
	if (token->redirect_in)
		input = token->redirect_in;
	else if (redir[0])
	{
		input = redir[0];
		redir[0] = 0;
	}
	else if (pre_fd)
		input = pre_fd[0];
	dup2(input, STDIN_FILENO);
	if (token->redirect_out)
		output = token->redirect_out;
	else if (redir[1])
		output = redir[1];
	else if (fd)
		output = fd[1];
	dup2(output, STDOUT_FILENO);
}

void	command_pipe(t_cmd *token, int *fd, int *pre_fd, int redirect)
{
	int	pid;
	static int	redir[2];

	if (redirect)
	{
		if (token->redirect_in)
			redir[0] = token->redirect_in;
		if (token->redirect_out)
			redir[1] = token->redirect_out;

		return ;
	}
	else
	{
		pid = fork();
		if (pid == -1)
			perror("Fork");
		if (!pid)
			command(token, fd, pre_fd, redir);
		else
		{
			waitpid(pid, NULL, 0);
			close(fd[0]);
			close(fd[1]);
			close(pre_fd[0]);
			close(pre_fd[1]);
		}
		exec(token->args);
	}
}

void	create_tree(int *pre_fd, t_node *token, int pid)
{
	int	fd[2];
	t_node	*left_test;
	t_node	*right_test;
	t_cmd	*cmd_r_test;

	left_test = token->left;
	right_test = token->right;
	cmd_r_test = token->right;
	if (pipe(fd) == -1)
		perror("Pipe");
	else if (!pid)
		run_tree(pre_fd, token, fd);
	else
	{
		waitpid(pid, NULL, 0);
		close(fd[0]);
		close(fd[1]);
	}
}

void	run_tree(int *pre_fd, t_node *token, int fd[2])
{
	int	pid;

	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pid && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, pre_fd, 0);
	else if (pid && token->type_left == REDIR)
		redirect((t_redir *)token->left, fd, pre_fd);
	else if (pid && token->type_left == AND)
		and_execute((t_node *)token->left, fd, pre_fd, 1);
	else if (pid && token->type_left == OR)
		or_execute((t_node *)token->left, fd, pre_fd, 1);
	else if (!pid && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right, 0);
	else if (!pid && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, fd, fd, 0);
	else if (!pid && token->type_right == REDIR)
		redirect((t_redir *)token->right, fd, pre_fd);
	else if (!pid && token->type_right == AND)
		and_execute((t_node *)token->right, fd, pre_fd, 1);
	else if (!pid && token->type_right == OR)
		or_execute((t_node *)token->right, fd, pre_fd, 1);
}

void	execution(void *tree, t_node_type type)
{
	int			pid;

	pid = fork();
	if (pid == -1)
		perror("Fork");

	if (!pid)
	{
		if (type == CMD)
			command_pipe((t_cmd *)tree, NULL, NULL, 0);
		else if (type == AND)
			and_execute((t_node *)tree, NULL, NULL, 0);
		else if (type == OR)
			or_execute((t_node *)tree, NULL, NULL, 0);
		else if (type == PIPE)
			create_tree(0, (t_node *)tree, pid);
		else if (type == REDIR)
			command_pipe((t_cmd *)tree, NULL, NULL, 1);
		exit(256);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
