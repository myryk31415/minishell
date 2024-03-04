/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:02:47 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 18:10:50 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node	*get_pipe(t_token *tokens)
{
	while (tokens && tokens->type != T_LPAREN)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens, 1);
		else if (tokens->type == T_PIPE)
			return (tokens);
		if (!tokens->next )
			return (tokens);
		tokens = tokens->next;
	}
	if (tokens->type == T_LPAREN)
		tokens = tokens->next;
	return (tokens);}

t_node_type	get_pipeline(t_token *token_first, void **head, t_cmd *redirects)
{
	t_token	*token_tmp;
	t_node	*node;

	(void)redirects;
	if (!token_first)
		return (ERROR);
	token_tmp = get_pipe(token_first);
	if (token_tmp->type == T_PIPE)
	{
		node = new_node();
		token_tmp = token_split(token_tmp, 1);
		token_delete(&token_tmp);
		node->type_left = get_cmd(token_first, &node->left, redirects);
		node->type_right = get_pipeline(token_tmp, &node->right, redirects);
		*head = node;
		return (PIPE);
	}
	return (get_cmd(token_first, head, redirects));
}
