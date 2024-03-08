/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/08 12:24:45 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_execute(t_node *token, int *fd, int *pre_fd, int i)
{
	int	id;
	int	status;

	id = fork();
	if (!i && !id && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, NULL, pre_fd);
	else if (!id && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, NULL, NULL);
	else if (token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, NULL);
	else if (token->type_left == OR)
	{
		waitpid(id, &status, 0);
		if (status == -1)
			or_execute((t_node *)token->left, fd, pre_fd, i);
	}
	else if (token->type_left == AND)
	{
		waitpid(id, &status, 0);
		if (status == 0)
			and_execute((t_node *)token->left, fd, pre_fd, i + 1);
	}
}

void	or_execute(t_node *token, int *fd, int *pre_fd, int i)
{
	int	id;
	int	status;

	id = fork();
	if (!i && !id && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, NULL, pre_fd);
	else if (!id && token->type_right == CMD)
		command_pipe((t_cmd *)token->right, NULL, NULL);
	else if (token->type_left == CMD)
		command_pipe((t_cmd *)token->left, fd, NULL);
	else if (token->type_left == OR)
	{
		waitpid(id, &status, 0);
		if (status == -1)
			or_execute((t_node *)token->left, fd, pre_fd, i);
	}
	else if (token->type_left == AND)
	{
		waitpid(id, &status, 0);
		if (status == 0)
			and_execute((t_node *)token->left, fd, pre_fd, i + 1);
	}
}
