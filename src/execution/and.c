/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/12 15:31:14 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_execute(t_node *token, int *fd, int *pre_fd, int status)
{
		int	id;

	id = fork();
	if (status == 1 && !id && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, NULL, 0);
	if (!id && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, NULL, NULL, 0);
	else if (status && !id && token->type_left == OR)
		or_execute((t_node *)token->left, fd, pre_fd, status);
	else if (!status && !id && token->type_left == AND)
		and_execute((t_node *)token->left, fd, pre_fd, status);
	else if (!id && token->type_left == PIPE)
		create_tree(0, (t_node *)token->left, 0);
	else if (!id && token->type_left == REDIR)
		redirect((t_redir *)token->left, fd, pre_fd);
	else if (!status && token->type_right == PIPE)
	{
		waitpid(id, &status, 0);
		create_tree(fd, (t_node *)token->right, 0);
	}
	else if (!status && token->type_right == CMD)
	{
		waitpid(id, &status, 0);
		command_pipe((t_cmd *)token->right, NULL, NULL, 0);
	}
	else if (status && token->type_right == OR)
	{
		waitpid(id, &status, 0);
		or_execute((t_node *)token->right, fd, pre_fd, status);
	}
	else if (!status && token->type_right == AND)
	{
		waitpid(id, &status, 0);
		and_execute((t_node *)token->right, fd, pre_fd, status);
	}
	else if (token->type_right == REDIR)
	{
		waitpid(id, &status, 0);
		redirect((t_redir *)token->right, fd, pre_fd);
	}
}

void	or_execute(t_node *token, int *fd, int *pre_fd, int status)
{
		int	id;

	id = fork();
	if (status == 1 && !id && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, NULL, 0);
	if (!id && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, NULL, NULL, 0);
	else if (status && !id && token->type_left == OR)
		or_execute((t_node *)token->left, fd, pre_fd, status);
	else if (!status && !id && token->type_left == AND)
		and_execute((t_node *)token->left, fd, pre_fd, status);
	else if (!id && token->type_left == PIPE)
		create_tree(0, (t_node *)token->left, 0);
	else if (!id && token->type_left == REDIR)
		redirect((t_redir *)token->left, fd, pre_fd);
	else if (!status && token->type_right == PIPE)
	{
		waitpid(id, &status, 0);
		create_tree(fd, (t_node *)token->right, 0);
	}
	else if (!status && token->type_right == CMD)
	{
		waitpid(id, &status, 0);
		command_pipe((t_cmd *)token->right, NULL, NULL, 0);
	}
	else if (status && token->type_right == OR)
	{
		waitpid(id, &status, 0);
		or_execute((t_node *)token->right, fd, pre_fd, status);
	}
	else if (!status && token->type_right == AND)
	{
		waitpid(id, &status, 0);
		and_execute((t_node *)token->right, fd, pre_fd, status);
	}
	else if (token->type_right == REDIR)
	{
		waitpid(id, &status, 0);
		redirect((t_redir *)token->right, fd, pre_fd);
	}
}
