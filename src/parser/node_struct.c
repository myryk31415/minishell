/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/02/27 17:36:43 by padam            ###   ########.fr       */
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

t_node_type get_next_node_two(t_token *tokens, void **head, t_cmd *redirects)
{
	if (!parens(tokens))
			return (get_pipeline(tokens, head));
	return (get_next_node(tokens, head, redirects_get(tokens, redirects)));
}

t_node_type	get_next_node(t_token *tokens, void **head, t_cmd *redirects)
{
	t_token	*tmp;
	int		operator_count;

	operator_count = count_operators(tokens);
	if (operator_count == 0)
		return (get_next_node_two(tokens, head, redirects));
	else
	{
		while (operator_count > 1)
		{

		}

		return ()
	}
}

t_node_type	tokens_to_tree(t_token *tokens, void **node_tree)
{
	t_node_type	node_type;
	t_cmd		*redirects;

	node_type = get_next_node(tokens, node_tree, redirects);
	return (node_type);
}
