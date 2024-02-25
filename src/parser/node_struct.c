/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/25 20:35:24 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*get_right_paren(t_token *tokens)
{
	int	paren_level;

	paren_level = 1;
	tokens = tokens->next
	while (paren_level && tokens)
	{
		if ()
	}
}

t_token_type	get_next_node(t_token *tokens, t_node *n_top)
{
	// t_node 		*node_left;
	// t_node_type	node_type_left;
	// t_node 		*node_right;
	// t_node		node_type_right;

	if (token->type == T_WORD)
	if (tokens->type == T_PARENTHESIS_OPEN)
		n_top->type_left == get_next_node(tokens->next, n_top->left);
}

t_token	*get_right_paren(t_token *tokens)
{
	int	paren_level;

	paren_level = 1;
	tokens = tokens->next
	while (paren_level && tokens)
	{
		if ()
	}
}
t_token_type	get_next_node(t_token *tokens, t_node *n_top)
{
	// t_node 		*node_left;
	// t_node_type	node_type_left;
	// t_node 		*node_right;
	// t_node		node_type_right;

	if (token->type == T_WORD)
	if (tokens->type == T_PARENTHESIS_OPEN)
		n_top->type_left == get_next_node(tokens->next, n_top->left);
}

t_token	*get_right_paren(t_token *tokens)
{
	int	paren_level;

	paren_level = 1;
	tokens = tokens->next
	while (paren_level && tokens)
	{
		if ()
	}
}

t_token_type	get_next_node(t_token *tokens, t_node *n_top)
{
	// t_node 		*node_left;
	// t_node_type	node_type_left;
	// t_node 		*node_right;
	// t_node		node_type_right;

	if (token->type == T_WORD)
	if (tokens->type == T_PARENTHESIS_OPEN)
		n_top->type_left == get_next_node(tokens->next, n_top->left);
}

t_node	*tokens_to_tree(t_token *tokens)
{
	t_node	*node_tree;

	node_tree = new_node();
	if (!node_tree)
		return (NULL);
	return (node_tree);
}
