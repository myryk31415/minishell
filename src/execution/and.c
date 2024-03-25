/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/25 13:15:39 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_left(t_node *token, int *status, int ***pipes, char ***env)
{
	int	*tmp_pipe;

	if (*status == 1)
	{
		tmp_pipe = *pipes[0];
		*pipes[0] = NULL;
		if (token->type_left == CMD)
			*status = command_pipe((t_cmd *)token->left, *pipes, 2, env);
		else if (token->type_left == REDIR)
			*status = redirect((t_redir *)token->left, *pipes, 0, *env);
		*pipes[0] = tmp_pipe;
		*pipes[1] = NULL;
	}
	else if (token->type_left == CMD)
		*status = command_pipe((t_cmd *)token->left, NULL, 2, env);
	else if (token->type_left == OR)
		*status = or_execute((t_node *)token->left, 0, env);
	else if (token->type_left == AND)
		*status = and_execute((t_node *)token->left, 0, *pipes, env);
	else if (token->type_left == PIPE)
		*status = create_tree(0, (t_node *)token->left, 0, *env);
	else if (token->type_left == REDIR)
		*status = redirect((t_redir *)token->left, NULL, 0, *env);
}

int	and_execute(t_node *token, int status, int **pipes, char ***env)
{
	and_left(token, &status, &pipes, env);
	if (!status && token->type_right == CMD)
		status = command_pipe((t_cmd *)token->right, pipes, 2, env);
	else if (!status && token->type_right == PIPE)
		status = create_tree(0, (t_node *)token->right, 0, *env);
	else if (!status && token->type_right == REDIR)
		status = redirect((t_redir *)token->right, pipes, 0, *env);
	return (status);
}


int	or_execute(t_node *token, int status, char ***env)
{
	if (token->type_left == CMD)
		status = command_pipe((t_cmd *)token->left, NULL, 2, env);
	else if (token->type_left == OR)
		status = or_execute((t_node *)token->left, 0, env);
	else if (token->type_left == AND)
		status = and_execute((t_node *)token->left, 0, NULL, env);
	else if (token->type_left == PIPE)
		status = create_tree(0, (t_node *)token->left, 0, *env);
	else if (token->type_left == REDIR)
		status = redirect((t_redir *)token->left, NULL, 0, *env);
	if (status == 256 && token->type_right == CMD)
		status = command_pipe((t_cmd *)token->right, NULL, 2, env);
	else if (status == 256 && token->type_right == PIPE)
		status = create_tree(0, (t_node *)token->right, 0, *env);
	else if (status == 256 && token->type_right == REDIR)
		status = redirect((t_redir *)token->right, NULL, 0, *env);
	return (status);
}
