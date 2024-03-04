/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 21:14:58 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	split_by_operator(t_token *token_last,
				void **head, t_cmd *redirects);

t_token	*get_operator(t_token *tokens)
{
	//token_deletion
	while (tokens)
	{
		if (tokens->type == T_RPAREN)
			tokens = skip_parens(tokens, -1);
		else if (tokens->type == T_AND || tokens->type == T_OR)
			return (tokens);
		if (!tokens->prev)
			return (tokens);
		tokens = tokens->prev;
	}
	if (tokens->type == T_LPAREN)
		tokens = tokens->next;
	return (tokens);
}

t_node_type	split_by_operator(t_token *token_last,
				void **head, t_cmd *redirects)
{
	t_token		*token_first;
	t_node		*node;
	t_node_type	return_value;

	if (!token_last)
		return (ERROR);
	token_first = get_operator(token_last);
	if (token_first->type == T_AND || token_first->type == T_OR)
	{
		node = new_node();
		if (token_first->type == T_AND)
			return_value = AND;
		else
			return_value = OR;
		token_delete(&token_first);
		node->type_left = split_by_operator(token_split(token_first, -1),
				&node->left, redirects);
		node->type_right = split_by_pipe(token_first, &node->right, redirects);
		*head = node;
		return (return_value);
	}
	return (split_by_pipe(token_first, head, redirects));
}

t_node_type	tokens_to_tree(t_token *token_last, void **node_tree)
{
	t_node_type	node_type;
	t_cmd		redirects;

	node_type = split_by_operator(token_last, node_tree, &redirects);
	cmd_free(&redirects);
	return (node_type);
}
