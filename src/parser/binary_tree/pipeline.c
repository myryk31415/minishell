/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:02:47 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 20:58:24 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type check_brackets(t_token *token_first, void **head, t_cmd *redirects)
{
	t_token_type	return_value;
	t_token 		*token_last;

	if (!token_first)
		return (ERROR);
	// redirects = redirects_get(token_first, redirects);
	if (token_first->type == T_LPAREN)
	{
		token_last = skip_parens(token_first, 1);
		if (!token_last || token_last->next)
		{
			cmd_free(redirects);
			return (ERROR);
		}
		token_last = token_last->prev;
		token_delete(&token_last->next);
		token_delete(&token_first);
		return_value = split_by_operator(token_last, head, redirects);
		cmd_free(redirects);
		return (return_value);
	}
	else
		return (get_cmd(token_first, head, redirects));
}

t_node	*get_pipe(t_token *tokens)
{
	//token_deletion
	while (tokens)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens, 1);
		else if (tokens->type == T_PIPE)
			return (tokens);
		if (!tokens->next )
			return (tokens);
		tokens = tokens->next;
	}
	if (tokens->type == T_RPAREN)
		tokens = tokens->prev;
	return (tokens);
}

t_node_type	split_by_pipe(t_token *token_first, void **head, t_cmd *redirects)
{
	t_token		*token_last;
	t_node		*node;

	if (!token_first)
		return (ERROR);
	token_last = get_pipe(token_first);
	if (token_last->type == T_PIPE)
	{
		node = new_node();
		token_delete(&token_last);
		token_split(token_last, -1);
		node->type_left = check_brackets(token_first, &node->left, redirects);
		node->type_right = split_by_pipe(token_last, &node->right, redirects);
		*head = node;
		return (PIPE);
	}
	return (check_brackets(token_first, head, redirects));
}

// t_node_type	get_pipeline(t_token *token_first, void **head, t_cmd *redirects)
// {
// 	t_token	*token_tmp;
// 	t_node	*node;

// 	(void)redirects;
// 	if (!token_first)
// 		return (ERROR);
// 	token_tmp = get_pipe(token_first);
// 	if (token_tmp->type == T_PIPE)
// 	{
// 		node = new_node();
// 		token_tmp = token_split(token_tmp, 1);
// 		token_delete(&token_tmp);
// 		node->type_left = get_cmd(token_first, &node->left, redirects);
// 		node->type_right = get_pipeline(token_tmp, &node->right, redirects);
// 		*head = node;
// 		return (PIPE);
// 	}
// 	return (get_cmd(token_first, head, redirects));
// }
