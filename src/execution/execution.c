/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/21 15:16:27 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command(t_cmd *token, int *fd, int *pre_fd)
{
	if (token->redirect_in)
		input(token->redirect_in);
	else
	{
		dup2(pre_fd[0], STDIN_FILENO);
		close(pre_fd[0]);
		close(pre_fd[1]);
	}
	if (token->redirect_out)
	{
		output(token->redirect_out);
		fd[1] = 0;
	}
	else
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	exec(token->args);
}

void	create_tree(int *pre_fd, t_node *token)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		perror("Pipe");
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pid == 0 && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right);
	else if (pid == 0 && token->type_right == CMD)
		command((t_cmd *)token->right, fd, pre_fd);
	else
		command((t_cmd *)token->left, fd, pre_fd);
}
