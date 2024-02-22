/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:44:23 by padam             #+#    #+#             */
/*   Updated: 2024/02/22 20:48:07 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node	*new_node(void)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node_group	*new_node_group(int nodes)
{
	t_node_group	*node_group;

	node_group = ft_calloc(1, sizeof(t_node_group));
	if (!node_group)
		return (NULL);
	node_group->node = ft_calloc(nodes, sizeof(t_node));
	node_group->type = ft_calloc(nodes, sizeof(t_token_type));
	return (node_group);
}
