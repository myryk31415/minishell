/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/01 11:03:29 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command(t_cmd *token, int *fd, int *pre_fd)
{
	int	err;

	err = 0;
	if (token->redirect_in)
		err = input(token->redirect_in, token->heredoc);
	else
	{
		dup2(pre_fd[0], STDIN_FILENO);
		close(pre_fd[0]);
		close(pre_fd[1]);
	}
	if (token->redirect_out && !err)
	{
		err = output(token->redirect_out, token->append);
		fd[1] = 0;
	}
	else
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	if (!err)
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
