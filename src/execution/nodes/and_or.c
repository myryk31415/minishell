/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/29 12:38:17 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_left(t_node *token, int **pipes, t_exec *exec)
{
	if (token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 2, exec);
	else if (token->type_left == REDIR)
		exec->exit_status = redirect((t_redir *)token->left, \
		pipes, 0, exec);
	if (token->type_left == OR)
		or_execute((t_node *)token->left, pipes, exec);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, pipes, exec);
	else if (token->type_left == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->left, exec, pipes);
	else if (token->type_right == ERROR)
		exec->exit_status = 2;
}

void	and_execute(t_node *token, int **pipes, t_exec *exec)
{
	and_left(token, pipes, exec);
	if (!exec->exit_status && token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 2, exec);
	else if (!exec->exit_status && token->type_right == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->right, \
			exec, pipes);
	else if (!exec->exit_status && token->type_right == REDIR)
		exec->exit_status = redirect((t_redir *)token->right, pipes, 0, exec);
	else if (!exec->exit_status && token->type_right == ERROR)
		exec->exit_status = 2;
}

void	or_left(t_node *token, int **pipes, t_exec *exec)
{
	if (token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 2, exec);
	else if (token->type_left == REDIR)
		exec->exit_status = redirect((t_redir *)token->left, \
		pipes, 0, exec);
	if (token->type_left == OR)
		or_execute((t_node *)token->left, pipes, exec);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, pipes, exec);
	else if (token->type_left == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->left, exec, pipes);
	else if (token->type_right == ERROR)
		exec->exit_status = 2;
}

void	or_execute(t_node *token, int **pipes, t_exec *exec)
{
	or_left(token, pipes, exec);
	if (exec->exit_status && token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 2, exec);
	else if (exec->exit_status && token->type_right == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->right, \
			exec, pipes);
	else if (exec->exit_status && token->type_right == REDIR)
		exec->exit_status = redirect((t_redir *)token->right, pipes, 0, exec);
	else if (exec->exit_status && token->type_right == ERROR)
		exec->exit_status = 2;
}
