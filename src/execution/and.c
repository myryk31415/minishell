/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/26 14:00:42 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_left(t_node *token, int status, int **pipes, t_exec *exec)
{
	if (token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 2, exec);
	else if (token->type_left == REDIR)
		exec->exit_status = redirect((t_redir *)token->left, \
		pipes, 0, *exec);
	if (token->type_left == OR)
		or_execute((t_node *)token->left, 1, pipes, exec);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, 1, pipes, exec);
	else if (token->type_left == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->left, *exec, NULL);
	else if (token->type_right == ERROR)
		exec->exit_status = 2;
	if (status == 1)
	{
		if (pipes)
		{
			close_pipe(pipes[1]);
			pipes[1] = NULL;
		}
	}
}

void	and_execute(t_node *token, int status, int **pipes, t_exec *exec)
{
	and_left(token, status, pipes, exec);
	if (!exec->exit_status && token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 2, exec);
	else if (!exec->exit_status && token->type_right == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->right, \
			*exec, pipes);
	else if (!exec->exit_status && token->type_right == REDIR)
		exec->exit_status = redirect((t_redir *)token->right, pipes, 0, *exec);
	else if (!exec->exit_status && token->type_right == ERROR)
		exec->exit_status = 2;
	// if (pipes && pipes[0])
	// 	close(pipes[0][1]);
}

void	or_left(t_node *token, int status, int **pipes, t_exec *exec)
{
	if (token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 2, exec);
	else if (token->type_left == REDIR)
		exec->exit_status = redirect((t_redir *)token->left, \
		pipes, 0, *exec);
	if (token->type_left == OR)
		or_execute((t_node *)token->left, 1, pipes, exec);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, 1, pipes, exec);
	else if (token->type_left == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->left, *exec, NULL);
	else if (token->type_right == ERROR)
		exec->exit_status = 2;
	if (status == 1)
	{
		if (pipes)
		{
			close_pipe(pipes[1]);
			pipes[1] = NULL;
		}
	}
}

void	or_execute(t_node *token, int status, int **pipes, t_exec *exec)
{
	or_left(token, status, pipes, exec);
	if (exec->exit_status && token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 2, exec);
	else if (exec->exit_status && token->type_right == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->right, \
			*exec, pipes);
	else if (exec->exit_status && token->type_right == REDIR)
		exec->exit_status = redirect((t_redir *)token->right, pipes, 0, *exec);
	else if (exec->exit_status && token->type_right == ERROR)
		exec->exit_status = 2;
	// if (pipes && pipes[0])
	// 	close(pipes[0][1]);
}
