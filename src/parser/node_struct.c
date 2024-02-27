/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/26 22:43:06 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


// void	get_pipeline(t_token	**tokens, t_cmd )
// {
// 	while (is_pipeline)
// }

// void	get_type(t_token **tokens, t_node *n_top)
// {
// 	if ((*tokens)->type == T_WORD)
// 		get_pipeline(tokens, n_top);
// 	if ((*tokens)->type == T_LPAREN)
// 		handle_parens(tokens, n_top);
// }

// t_node_type	get_next_node(t_token *tokens, t_node *n_top)
// {
// 	// t_node 		*node_left;
// 	// t_node_type	node_type_left;
// 	// t_node 		*node_right;
// 	// t_node		node_type_right;

// 	return (AND);
// }

t_node	*tokens_to_tree(t_token *tokens)
{
	t_node	*node_tree;

	(void) tokens;
	node_tree = new_node();
	if (!node_tree)
		return (NULL);
	return (node_tree);
}
