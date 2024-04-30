/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:44:23 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 18:47:54 by antonweizma      ###   ########.fr       */
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

t_redir	*new_redir_node(void)
{
	t_redir	*node;

	node = ft_calloc(1, sizeof(t_redir));
	if (!node)
		return (NULL);
	node->next = NULL;
	return (node);
}

t_token	*delete_parens(t_token *token_first)
{
	t_token	*token_last;

	token_last = skip_parens(token_first, 1);
	if (token_last->next)
	{
		print_syntax_err(token_last);
		return (NULL);
	}
	token_last = token_last->prev;
	token_delete(&token_last->next);
	token_delete(&token_first);
	if (!token_first)
	{
		print_syntax_err(NULL);
		return (NULL);
	}
	return (token_last);
}
