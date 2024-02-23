/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/23 19:20:35 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node	*get_next_node(t_token *tokens)
{
	int		operator_count;
	t_node	*node;

	operator_count = count_operators(tokens);
	
}

t_node	*tokens_to_tree(t_token *tokens)
{
	t_node	*node_tree;

	node_tree = new_node();
	if (!node_tree)
		return (NULL);
	return (node_tree);
}
