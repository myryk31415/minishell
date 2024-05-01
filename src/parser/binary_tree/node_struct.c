/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 21:44:16 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	get_cmd(t_token *token_first, void **head, t_cmd *redirects)
{
	int	word_count;
	int	i;

	i = 0;
	if (!redirects)
		return (ERROR);
	word_count = count_words(token_first);
	if (word_count == -1 || (word_count == 0 && !redirects))
	{
		if (word_count == 0)
			print_syntax_err(NULL);
		return (cmd_free(redirects), ERROR);
	}
	redirects->args = ft_calloc(word_count + 1, sizeof(char *));
	if (!redirects->args)
		return (err_pars("malloc", redirects, &token_first));
	while (token_first)
	{
		if (token_first->value)
			redirects->args[i++] = token_first->value;
		token_first->value = NULL;
		token_delete(&token_first);
	}
	*head = redirects;
	return (CMD);
}

t_node_type	check_parens(t_token *token_first, void **head)
{
	t_token	*token_last;
	t_cmd	*redirects;
	t_redir	*node;

	if (!token_first || redirects_get(&token_first, &redirects) == -1)
		return (ERROR);
	if (token_first && token_first->type == T_LPAREN)
	{
		node = new_redir_node();
		if (!node)
			return (err_pars("malloc", redirects, &token_first));
		node->redirects = redirects;
		node->type = REDIR;
		*head = node;
		token_last = delete_parens(token_first);
		if (!token_last)
			return (node_tree_delete(node, REDIR), ERROR);
		node->type = split_by_operator(token_last, &node->next);
		if (node->type == ERROR)
			return (node_tree_delete(node, REDIR), ERROR);
		return (REDIR);
	}
	return (get_cmd(token_first, head, redirects));
}

t_node_type	split_by_pipe(t_token *token_first, void **head)
{
	t_token		*token_last;
	t_node		*node;

	token_last = get_pipe(token_first);
	if (!token_first || !token_last)
		return (ERROR);
	if (token_last->type == T_PIPE)
	{
		node = new_node();
		if (!node)
			return (err_pars("malloc", NULL, &token_first));
		*head = node;
		if (!token_last->next || !token_last->prev)
			return (print_syntax_err(token_last), free(node), ERROR);
		token_delete(&token_last);
		token_split(token_last, -1);
		node->type_left = check_parens(token_first, &node->left);
		if (node->type_left == ERROR)
			return (token_delete_all(&token_last), free(node), ERROR);
		node->type_right = split_by_pipe(token_last, &node->right);
		if (node->type_right == ERROR)
			return (node_tree_delete(*head, PIPE), ERROR);
		return (PIPE);
	}
	return (check_parens(token_first, head));
}

t_node_type	dies_das(t_token *token, void **head)
{
	t_node_type	return_value;

	if (!token->prev || !token->next)
	{
		print_syntax_err(token);
		return (ERROR);
	}
	*head = new_node();
	if (!head)
	{
		token_delete_all(&token);
		return (err_pars("malloc", NULL, &token));
	}
	if (token->type == T_AND)
		return_value = AND;
	else
		return_value = OR;
	return (return_value);
}

t_node_type	split_by_operator(t_token *token_last, void **head)
{
	t_token		*token_first;
	t_node		*node;
	t_node_type	return_value;

	token_first = get_operator(token_last);
	if (!token_first || !token_last)
		return (ERROR);
	if (token_first->type == T_AND || token_first->type == T_OR)
	{
		return_value = dies_das(token_first, head);
		if (return_value == ERROR)
			return (ERROR);
		node = *head;
		token_delete(&token_first);
		node->type_left = split_by_operator(token_split(token_first, -1),
				&node->left);
		if (node->type_left == ERROR)
			return (token_delete_all(&token_last), free(node), ERROR);
		node->type_right = split_by_pipe(token_first, &node->right);
		if (node->type_right == ERROR)
			return (node_tree_delete(*head, return_value), ERROR);
		return (return_value);
	}
	return (split_by_pipe(token_first, head));
}
