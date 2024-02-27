/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/27 19:48:03 by padam            ###   ########.fr       */
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

t_node_type	get_pipeline(t_token	*tokens, void **head, t_cmd *redirects)
{
	(void)redirects;
	(void)tokens;
	head = NULL;
	(void)head;
	return (PIPE);
}

t_node_type get_next_node_two(t_token *tokens, void **head, t_cmd *redirects)
{
	t_token *paren;
	t_token *token_first;

	paren = NULL;
	token_first = get_paren(tokens, paren);
	if (!paren)
			return (get_pipeline(tokens, head, redirects));
	redirects = redirects_get(token_first, redirects);
	return (get_next_node(paren->prev, head, redirects));
}

t_node_type	get_next_node(t_token *token_last, void **head, t_cmd *redirects)
{
	t_token	*token_first;
	t_node	*node;

	//fix this
	if (!token_last)
		return (AND);
	token_first = get_operator(token_last);
	if (token_first->type == T_AND || token_first->type == T_OR)
	{
		node = new_node();
		node->type_right = get_next_node_two(token_last, &node->right, redirects);
		node->type_left = get_next_node_two(token_first->next, &node->left, redirects);
		*head = node;
		if (token_first->type == T_AND)
			return (AND);
		return (OR);
	}
	return (get_next_node_two(token_first, head, redirects));
}

t_node_type	tokens_to_tree(t_token *token_last, void **node_tree)
{
	t_node_type	node_type;
	t_cmd		redirects;

	node_type = get_next_node(token_last, node_tree, &redirects);
	return (node_type);
}
