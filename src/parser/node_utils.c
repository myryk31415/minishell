/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:44:23 by padam             #+#    #+#             */
/*   Updated: 2024/02/23 01:11:51 by padam            ###   ########.fr       */
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

t_node_group	*new_node_group(int operator_count)
{
	t_node_group	*node_group;

	node_group = ft_calloc(1, sizeof(t_node_group));
	if (!node_group)
		return (NULL);
	node_group->node = ft_calloc(operator_count, sizeof(t_node));
	if (!node_group->node)
	{
		free(node_group);
		return (NULL);
	}
	node_group->type = ft_calloc(operator_count, sizeof(t_token_type));
	if (!node_group->type)
	{
		free(node_group->node);
		free(node_group);
		return (NULL);
	}
	return (node_group);
}

/*
 * @brief Counts "T_AND" and "T_OR" tokens in the token list
*/
int	count_operators(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_AND || tokens->type == T_OR)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
