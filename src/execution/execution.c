/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/19 17:47:39 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// int	execution(t_node *token, t_redirect_in *input, t_redirect_out *output)
// {
// 	if (token->type == PIPE)
// 		create_tree(token->left, token->right);
// 	if (token->type == CMD)
// 		exec(token->command);
// }

void	cmd(char *cmd, int *fd, int *pre_fd)
{
	if (pre_fd)
	{
		dup2(pre_fd[0], STDIN_FILENO);
		close(pre_fd[0]);
		close(pre_fd[1]);
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	exec(cmd);
}

void	create_tree(int *pre_fd, t_node *token, int commands)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		perror("Pipe");
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pid == 0 && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right, commands);
	else if (pid != 0 && token->type_left == REDIR_IN)
		input((t_redirect_in *)token->left, fd);
	else if (pid == 0 && token->type_right == REDIR_OUT)
		output((t_redirect_out *)token->right, fd);
	else
		cmd((char *)token->left, fd, pre_fd);
}
