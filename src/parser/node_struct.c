/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/23 01:11:07 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_group	*tokens_to_tree(t_token *tokens)
{
	t_node_group	*node_tree;
	int				operator_count;

	operator_count = count_operators(tokens);
	node_tree = new_node_group(operator_count);
	if (!node_tree)
		return (NULL);
	return (node_tree);
}
