/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/03/08 01:07:18 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	split_by_operator(t_token *token_last, void **head,
				t_cmd *redirects, bool *new_process);

t_node_type	get_cmd(t_token *token_first, void **head, t_cmd *redirects)
{
	int	word_count;
	int	i;

	i = 0;
	if (!redirects)
		return (err_pars("malloc failed", redirects, token_first));
	word_count = count_words(token_first);
	redirects->args =  ft_calloc(word_count + 1, sizeof(char *));
	if (word_count <= 0 || !redirects->args)
	{
		cmd_free(redirects);
		return (ERROR);
	}
	while (token_first)
	{
		redirects->args[i++] = token_first->value;
		token_delete(&token_first);
	}
	*head = redirects;
	return (CMD);
}

t_node_type	check_brackets(t_token *token_first, void **head,
				t_cmd *redirects, bool *new_process)
{
	t_node_type	return_value;
	t_token		*token_last;

	if (!token_first)
		return (ERROR);
	redirects = redirects_get(&token_first, redirects);
	if (token_first->type == T_LPAREN)
	{
		if (new_process)
			*new_process = true;
		token_last = skip_parens(token_first, 1);
		if (!token_last || token_last->next || !redirects)
		{
			cmd_free(redirects);
			return (ERROR);
		}
		token_last = token_last->prev;
		token_delete(&token_last->next);
		token_delete(&token_first);
		return_value = split_by_operator(token_last, head, redirects, NULL);
		cmd_free(redirects);
		return (return_value);
	}
	else
		return (get_cmd(token_first, head, redirects));
}

t_node_type	split_by_pipe(t_token *token_first, void **head,
				t_cmd *redirects, bool *new_process)
{
	t_token		*token_last;
	t_node		*node;

	token_last = get_pipe(token_first);
	if (!token_first || !token_last)
		return (ERROR);
	if (token_last->type == T_PIPE)
	{
		node = new_node();
		token_delete(&token_last);
		token_split(token_last, -1);
		node->type_left = check_brackets(token_first, &node->left, redirects,
				&node->new_process_left);
		node->type_right = split_by_pipe(token_last, &node->right, redirects ,
				&node->new_process_right);
		*head = node;
		if (node->type_left == ERROR || node->type_right == ERROR)
			return (ERROR);
		return (PIPE);
	}
	return (check_brackets(token_first, head, redirects, new_process));
}

t_node_type	split_by_operator(t_token *token_last, void **head,
				t_cmd *redirects, bool *new_process)
{
	t_token		*token_first;
	t_node		*node;
	t_node_type	return_value;

	token_first = get_operator(token_last);
	if (!token_first || !token_last)
		return (ERROR);
	if (token_first->type == T_AND || token_first->type == T_OR)
	{
		node = new_node();
		if (token_first->type == T_AND)
			return_value = AND;
		else
			return_value = OR;
		token_delete(&token_first);
		node->type_left = split_by_operator(token_split(token_first, -1),
				&node->left, redirects , &node->new_process_left);
		node->type_right = split_by_pipe(token_first, &node->right, redirects,
				 &node->new_process_right);
		if (node->type_left == ERROR || node->type_right == ERROR)
			return (ERROR);
		*head = node;
		return (return_value);
	}
	return (split_by_pipe(token_first, head, redirects, new_process));
}

t_node_type	tokens_to_tree(t_token *token_last, void **node_tree)
{
	t_node_type	node_type;
	t_cmd		redirects; // no pointer

	redirects.args = NULL;
	redirects.redirect_in = NULL;
	redirects.redirect_out = NULL;
	redirects.heredoc = NULL;
	redirects.append = NULL;
	node_type = split_by_operator(token_last, node_tree, &redirects, NULL);
	return (node_type);
}
