/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:10:56 by padam             #+#    #+#             */
/*   Updated: 2024/03/13 15:12:10 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	cmd_free(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_str_array(cmd->args);
	if (cmd->redirect_in)
		close(cmd->redirect_in);
	if (cmd->redirect_out)
		close(cmd->redirect_out);
	free(cmd);
}

void	node_tree_delete(void *node, t_node_type type)
{
	if (type == CMD)
		cmd_free((t_cmd *)node);
	else if (type == REDIR)
	{
		free(((t_redir *)node)->redirects);
		node_tree_delete(((t_redir *)node)->next, ((t_redir *)node)->type);
		free(node);
	}
	else if (type == PIPE || type == AND || type == OR)
	{
		node_tree_delete(((t_node *)node)->left, ((t_node *)node)->type_left);
		node_tree_delete(((t_node *)node)->right, ((t_node *)node)->type_right);
		free(node);
	}
}
