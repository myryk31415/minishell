/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:10:56 by padam             #+#    #+#             */
/*   Updated: 2024/04/28 13:10:08 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(char **arr, int *type)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] || type[i])
		free(arr[i++]);
	free(arr);
	free(type);
}

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
	free_redir(cmd->redirects, cmd->redirect_type);
	free(cmd);
}

void	node_tree_delete(void *node, t_node_type type)
{
	if (!node)
		return ;
	if (type == CMD)
		cmd_free((t_cmd *)node);
	else if (type == REDIR)
	{
		cmd_free(((t_redir *)node)->redirects);
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

void	free_env(char ***env)
{
	int	i;

	i = -1;
	while (env[0][++i])
		free(env[0][i]);
	free(env[0]);
	free (env);
	env = NULL;
}
