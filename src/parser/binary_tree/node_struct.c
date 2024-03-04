/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 17:02:31 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	get_next_node(t_token *token_last, void **head, t_cmd *redirects);


// void	get_type(t_token **tokens, t_node *n_top)
// {
// 	if ((*tokens)->type == T_WORD)
// 		get_pipeline(tokens, n_top);
// 	if ((*tokens)->type == T_LPAREN)
// 		handle_parens(tokens, n_top);
// }

t_node_type check_brackets(t_token *token_last, void **head, t_cmd *redirects)
{
	t_token *paren;
	t_token *token_first;

	paren = NULL;
	token_first = get_paren(token_last, &paren);
	if (!paren)
			return (get_pipeline(token_first, head, redirects));
	// redirects = redirects_get(token_first, redirects);
	return (get_next_node(paren->prev, head, redirects));
}

t_node_type	get_next_node(t_token *token_last, void **head, t_cmd *redirects)
{
	t_token		*token_first;
	t_node		*node;
	t_node_type return_value;

	//fix this
	if (!token_last)
		return (ERROR);
	token_first = get_operator(token_last);
	if (token_first->type == T_AND || token_first->type == T_OR)
	{
		node = new_node();
		node->type_right = check_brackets(token_last, &node->right, redirects);
		if (token_first->type == T_AND)
			return_value = AND;
		else
			return_value = OR;
		token_delete(&token_first);
		node->type_left = get_next_node(token_split(token_first, -1), &node->left, redirects);
		*head = node;
		return (return_value);
	}
	return (check_brackets(token_last, head, redirects));
}

t_node_type	tokens_to_tree(t_token *token_last, void **node_tree)
{
	t_node_type	node_type;
	t_cmd		redirects;

	node_type = get_next_node(token_last, node_tree, &redirects);
	return (node_type);
}
