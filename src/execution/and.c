/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/11 13:19:13 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_execute(t_node *token, int *fd, int *pre_fd, int status)
{
	int	id;

	id = fork();
	if (status == 1 && !id && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, NULL, pre_fd);
	if (!status && !id && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, NULL, NULL);
	else if (!id && token->type_left == OR)
	{
		or_execute((t_node *)token->left, fd, pre_fd, status);
	}
	else if (!id && token->type_left == AND)
	{
		and_execute((t_node *)token->left, fd, pre_fd, status);
	}
	else if (status == 1 && token->type_right == CMD)
	{
		waitpid(id, &status, 0);
		command_pipe((t_cmd *)token->right, NULL, pre_fd);
	}
	else if (!status && token->type_right == CMD)
	{
		waitpid(id, &status, 0);
		command_pipe((t_cmd *)token->right, NULL, NULL);
	}
	else if (token->type_right == OR)
	{
		waitpid(id, &status, 0);
		or_execute((t_node *)token->right, fd, pre_fd, status);
	}
	else if (token->type_right == AND)
	{
		waitpid(id, &status, 0);
		and_execute((t_node *)token->right, fd, pre_fd, status);
	}
}

void	or_execute(t_node *token, int *fd, int *pre_fd, int status)
{
	int	id;

	id = fork();
	if (status == 1 && !id && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, NULL, pre_fd);
	else if (status == 256 && !id && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, NULL, NULL);
	else if (status == 256 && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, NULL);
	else if (token->type_left == OR)
	{
		waitpid(id, &status, 0);
		or_execute((t_node *)token->left, fd, pre_fd, status);
	}
	else if (token->type_left == AND)
	{
		waitpid(id, &status, 0);
		and_execute((t_node *)token->left, fd, pre_fd, status);
	}
}
