/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:44:23 by padam             #+#    #+#             */
/*   Updated: 2024/03/21 22:33:26 by antonweizma      ###   ########.fr       */
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

// /*
//  * @brief Counts "T_AND" and "T_OR" tokens in the token list
// */
// int	count_operators(t_token *tokens)
// {
// 	int	operator_count;

// 	operator_count = 0;
// 	while (tokens)
// 	{
// 		if (is_operator(tokens->type))
// 			operator_count++;
// 		else if (tokens->type == T_LPAREN)
// 			tokens = skip_parens(tokens, 1);
// 		tokens = tokens->next;
// 	}
// 	return (operator_count);
// }

// t_token	*get_paren(t_token *tokens, t_token **paren)
// {
// 	//make cleaner, maybe dont need token_first
// 	while (tokens && tokens->prev && tokens->prev->type != T_OR
// 		&& tokens->prev->type != T_AND)
// 	{
// 		if (tokens->type == T_RPAREN)
// 		{
// 			*paren = tokens;
// 			tokens = skip_parens(*paren, -1);
// 		}
// 		if (tokens->prev && tokens->prev->type != T_OR
// 			&& tokens->prev->type != T_AND)
// 			tokens = tokens->prev;
// 	}
// 	return (tokens);
// }
