/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/22 20:34:23 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * @brief Counts "T_AND" and "T_OR" tokens in the token list
*/
int	count_children(t_token *tokens)
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

t_node_group	*tokens_to_node_tree(t_token *tokens)
{
	t_node_group	*node_tree;
	int				node_count;

	node_count = count_children(tokens);
	node_tree = new_node_group();
	if (!node_tree)
		return (NULL);
	return (node_tree);
}
