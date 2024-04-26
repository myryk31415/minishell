/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:39:18 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/26 12:07:14 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	and_left(t_node *token, int status, int **pipes, t_exec *exec)
{
	if (status == 1 && pipes)
	{
		if (pipes)
			pipes[0] = NULL;
		if (token->type_left == CMD)
			command((t_cmd *)token->left, pipes, 2, exec);
		else if (token->type_left == REDIR)
			exec->exit_status = redirect((t_redir *)token->left, \
			pipes, 0, *exec);
	}
	else if (token->type_left == CMD)
		command((t_cmd *)token->left, NULL, 2, exec);
	else if (token->type_left == REDIR)
		exec->exit_status = redirect((t_redir *)token->left, NULL, 0, *exec);
	if (token->type_left == OR)
		or_execute((t_node *)token->left, 0, pipes, exec);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, 0, pipes, exec);
	else if (token->type_left == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->left, *exec, NULL);
	else if (token->type_right == ERROR)
		exec->exit_status = 2;
}

void	and_execute(t_node *token, int status, int **pipes, t_exec *exec)
{
	int	*tmp_pipe;

	tmp_pipe = NULL;
	if (pipes)
		tmp_pipe = pipes[0];
	and_left(token, status, pipes, exec);
	if (!exec->exit_status && pipes && token->type_right == CMD)
	{
		pipes[0] = tmp_pipe;
		close_pipe(pipes[1]);
		pipes[1] = NULL;
		command((t_cmd *)token->right, pipes, 2, exec);
	}
	else if (!exec->exit_status && token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 2, exec);
	else if (!exec->exit_status && token->type_right == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->right, *exec, NULL);
	else if (!exec->exit_status && token->type_right == REDIR)
		exec->exit_status = redirect((t_redir *)token->right, pipes, 0, *exec);
	else if (!exec->exit_status && token->type_right == ERROR)
		exec->exit_status = 2;
}

void	or_left(t_node *token, int status, int **pipes, t_exec *exec)
{
	if (status == 1)
	{
		if (pipes)
			pipes[0] = NULL;
		if (token->type_left == CMD)
			command((t_cmd *)token->left, pipes, 2, exec);
		else if (token->type_left == REDIR)
			exec->exit_status = redirect((t_redir *)token->left, \
			pipes, 0, *exec);
	}
	else if (token->type_left == CMD)
		command((t_cmd *)token->left, NULL, 2, exec);
	else if (token->type_left == OR)
		or_execute((t_node *)token->left, 0, pipes, exec);
	else if (token->type_left == AND)
		and_execute((t_node *)token->left, 0, pipes, exec);
	else if (token->type_left == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->left, *exec, NULL);
	else if (token->type_left == REDIR)
		exec->exit_status = redirect((t_redir *)token->left, NULL, 0, *exec);
}

void	or_execute(t_node *token, int status, int **pipes, t_exec *exec)
{
	int	*tmp_pipe;

	tmp_pipe = NULL;
	if (pipes)
		tmp_pipe = pipes[0];
	or_left(token, status, pipes, exec);
	if (exec->exit_status && pipes && token->type_right == CMD)
	{
		pipes[0] = tmp_pipe;
		if (pipes[1])
		{
			close(pipes[1][0]);
			close(pipes[1][1]);
		}
		pipes[1] = NULL;
		command((t_cmd *)token->right, pipes, 2, exec);
	}
	else if (exec->exit_status && token->type_right == CMD)
		command((t_cmd *)token->right, pipes, 2, exec);
	else if (exec->exit_status && token->type_right == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->right, *exec, NULL);
	else if (exec->exit_status && token->type_right == REDIR)
		exec->exit_status = redirect((t_redir *)token->right, pipes, 0, *exec);

}
