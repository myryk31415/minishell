/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/22 18:28:02 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	and_execute(t_node *token, int **pipes, int status, char **env)
{
	if (status == 1 && token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, pipes, 0, env);
	else if (token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, NULL, 0, env);
	else if (token->type_left == OR)
		status = or_execute((t_node *)token->left, pipes, 0, env);
	else if (token->type_left == AND)
		status = and_execute((t_node *)token->left, pipes, 0, env);
	else if (token->type_left == PIPE)
		status = create_tree(0, (t_node *)token->left, 0, env);
	else if (token->type_left == REDIR)
		status = redirect((t_redir *)token->left, pipes, 0, env);
	if (!status && token->type_right == CMD)
		status = command_pipe((t_cmd *)token->right, NULL, 0, env);
	else if (!status && token->type_right == PIPE)
		status = create_tree(0, (t_node *)token->right, 0, env);
	else if (!status && token->type_right == REDIR)
		status = redirect((t_redir *)token->right, pipes, 0, env);
	return (status);
}


int	or_execute(t_node *token, int **pipes, int status, char **env)
{
	if (status == 1 && token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, pipes, 0, env);
	else if (token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, NULL, 0, env);
	else if (token->type_left == OR)
		status = or_execute((t_node *)token->left, pipes, 0, env);
	else if (token->type_left == AND)
		status = and_execute((t_node *)token->left, pipes, 0, env);
	else if (token->type_left == PIPE)
		status = create_tree(0, (t_node *)token->left, 0, env);
	else if (token->type_left == REDIR)
		status = redirect((t_redir *)token->left, pipes, 0, env);
	if (status == 256 && token->type_right == CMD)
		status = command_pipe((t_cmd *)token->right, NULL, 0, env);
	else if (status == 256 && token->type_right == PIPE)
		status = create_tree(0, (t_node *)token->right, 0, env);
	else if (status == 256 && token->type_right == REDIR)
		status = redirect((t_redir *)token->right, pipes, 0, env);
	return (status);
}
