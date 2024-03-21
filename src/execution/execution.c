/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/21 09:00:00 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command(t_cmd *token, int *fd, int *pre_fd, int *redir)
{
	int	input;
	int	output;

	input = 0;
	output = 1;
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

int	command_pipe(t_cmd *token, int *fd, int *pre_fd, int redirect)
{
	int			id;
	static int	redir[2];
	int			status;

	if (redirect == 1)
	{
		if (token->redirect_in)
			redir[0] = token->redirect_in;
		if (token->redirect_out)
			redir[1] = token->redirect_out;
		return (0);
	}
	else
	{
		status = is_builtin(token, fd, pre_fd, redir);
		if (status == 1)
		{
			if (redirect == 2)
				id = 0;
			else
			{
				id = fork();
				if (id == -1)
					perror("Fork");
			}
			if (!id)
			{
				command(token, fd, pre_fd, redir);
				exec(token->args);
			}
			else
			{
				waitpid(id, &status, 0);
				if (fd)
				{
					close(fd[0]);
					close(fd[1]);
				}
				if (pre_fd)
				{
					close(pre_fd[0]);
					close(pre_fd[1]);
				}
			}
		}
		return (status);
	}
	return (256);
}

int	create_tree(int *pre_fd, t_node *token, int status)
{
	int	fd[2];
	int	pid;

	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pipe(fd) == -1)
		perror("Pipe");
	else if (!pid)
		run_tree(pre_fd, token, fd);
	else
	{
		waitpid(pid, &status, 0);
		close(fd[0]);
		close(fd[1]);
	}
	return (status);
}

void	run_tree(int *pre_fd, t_node *token, int fd[2])
{
	int		pid;
	t_cmd	*left;
	t_cmd	*right;
	int	status;
	left = (t_cmd *)token->left;
	right = (t_cmd *)token->right;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid && token->type_left == CMD)
		command_pipe((t_cmd *)token->right, fd, pre_fd, 2);
	else if (!pid && token->type_left == 	REDIR)
		redirect((t_redir *)token->right, fd, pre_fd, 0);
	else if (pid && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right, 0);
	else if (pid && token->type_right == CMD)
		command_pipe((t_cmd *)token->left, 0, fd, 2);
	else if (pid && token->type_right == REDIR)
		redirect((t_redir *)token->left, fd, pre_fd, 0);
}

void	execution(void *tree, t_node_type type)
{
	if (type == CMD)
		command_pipe((t_cmd *)tree, NULL, NULL, 0);
	else if (type == AND)
		and_execute((t_node *)tree, NULL, NULL, 0);
	else if (type == OR)
		or_execute((t_node *)tree, NULL, NULL, 0);
	else if (type == PIPE)
		create_tree(0, (t_node *)tree, 0);
	else if (type == REDIR)
		redirect((t_redir *)tree, NULL, NULL, 0);}
