/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/17 12:51:48 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	execution_start(t_node *token, t_redirect_in *input, t_redirect_out *output)
{

}

int	execution(t_node *token, t_redirect_in *input, t_redirect_out *output)
{
	if (token->type == PIPE)
		create_tree(token->left, token->right);
	if (token->type == CMD)
		exec(token->command);
}

void	cmd(t_node *left, int *fd, int *pre_fd)
{
	dup2(fd[0], STDIN_FILENO);
	dup2(pre_fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(pre_fd[0]);
	close(pre_fd[1]);
	exec(left->command);
}

void	input(t_redirect_in *input, int *pre_fd)
{
	int	file;

	file = 0;

}

void	create_tree(int *pre_fd, t_node *token,
		t_redirect_in *input, t_redirect_out *output)
{
	int	fd[2];
	int	id;

	if (pipe(fd) == -1)
		perror("Pipe");
	id = fork();
	if (pid == -1)
		perror("Fork");
	if (pid == 0 && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right, input, output);
	else if (pid != 0 && token->type_left == REDIR_IN)
		input(, fd);
	else if (pid != 0 && token->type_right == REDIR_OUT)
		output(args, fd);
	else
		cmd(token->left, fd, pre_fd);

}
