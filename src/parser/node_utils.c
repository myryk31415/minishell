/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:44:23 by padam             #+#    #+#             */
/*   Updated: 2024/02/27 19:48:57 by padam            ###   ########.fr       */
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

/*
 * @brief Counts "T_AND" and "T_OR" tokens in the token list
*/
int	count_operators(t_token *tokens)
{
	int	operator_count;

	operator_count = 0;
	while (tokens)
	{
		if (is_operator(tokens->type))
			operator_count++;
		else if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens, 1);
		tokens = tokens->next;
	}
	return (operator_count);
}

t_token	*get_paren(t_token *tokens, t_token *paren)
{
	while (tokens && tokens->prev)
	{
		if (tokens->type == T_LPAREN)
		{
			paren = tokens;
			tokens = skip_parens(paren, -1);
		}
		tokens = tokens->prev;
	}
	return (tokens);
}
