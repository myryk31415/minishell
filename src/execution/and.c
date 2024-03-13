/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/13 20:35:58 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	and_execute(t_node *token, int *fd, int *pre_fd, int status)
{
	if (status == 1 && token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, fd, NULL, 0);
	else if (token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, NULL, NULL, 0);
	else if (token->type_left == OR)
		status = or_execute((t_node *)token->left, fd, pre_fd, 0);
	else if (token->type_left == AND)
		status = and_execute((t_node *)token->left, fd, pre_fd, 0);
	else if (token->type_left == PIPE)
		status = create_tree(0, (t_node *)token->left, 0);
	else if (token->type_left == REDIR)
		status = redirect((t_redir *)token->left, fd, pre_fd, 0);
	if (!status && token->type_right == CMD)
		status = command_pipe((t_cmd *)token->right, NULL, NULL, 0);
	else if (!status && token->type_right == PIPE)
		status = create_tree(0, (t_node *)token->right, 0);
	else if (!status && token->type_right == REDIR)
		status = redirect((t_redir *)token->right, fd, pre_fd, 0);
	return (status);
}


int	or_execute(t_node *token, int *fd, int *pre_fd, int status)
{
	if (status == 1 && token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, fd, NULL, 0);
	else if (token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, NULL, NULL, 0);
	else if (token->type_left == OR)
		status = or_execute((t_node *)token->left, fd, pre_fd, 0);
	else if (token->type_left == AND)
		status = and_execute((t_node *)token->left, fd, pre_fd, 0);
	else if (token->type_left == PIPE)
		status = create_tree(0, (t_node *)token->left, 0);
	else if (token->type_left == REDIR)
		status = redirect((t_redir *)token->left, fd, pre_fd, 0);
	if (status == 256 && token->type_right == CMD)
		status = command_pipe((t_cmd *)token->right, NULL, NULL, 0);
	else if (status == 256 && token->type_right == PIPE)
		status = create_tree(0, (t_node *)token->right, 0);
	else if (status == 256 && token->type_right == REDIR)
		status = redirect((t_redir *)token->right, fd, pre_fd, 0);
	return (status);
}
