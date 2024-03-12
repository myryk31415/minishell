/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/11 16:12:38 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command_pipe(t_cmd *token, int *fd, int *pre_fd)
{
	int	err;

	err = 0;
	if (token->redirect_in)
	{
		err = input(token->redirect_in, token->heredoc);
	}
	else if (pre_fd)
	{
		dup2(pre_fd[0], STDIN_FILENO);
		close(pre_fd[0]);
		close(pre_fd[1]);
	}
	if (token->redirect_out && !err)
	{
		err = output(token->redirect_out, token->append);
		fd = NULL;
	}
	else if (fd)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	if (!err)
		exec(token->args);
}

void	create_tree(int *pre_fd, t_node *token, int pid)
{
	int	fd[2];

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
	ft_printf("hello\n");
	if (pid && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, pre_fd);
	else if (pid == 0 && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right, 0);
	else if (pid == 0 && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, fd, pre_fd);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, fd, pre_fd, 1);
	else if (token->type_left == OR)
		or_execute((t_node *)token->left, fd, pre_fd, 1);
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
			command_pipe((t_cmd *)tree, NULL, NULL);
		else if (type == AND)
			and_execute((t_node *)tree, NULL, NULL, 0);
		else if (type == OR)
			or_execute((t_node *)tree, NULL, NULL, 0);
		else if (type == PIPE)
			create_tree(0, (t_node *)tree, pid);
		exit(256);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
