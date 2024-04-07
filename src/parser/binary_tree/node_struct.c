/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:38:13 by padam             #+#    #+#             */
/*   Updated: 2024/04/08 01:01:52 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_node_type	split_by_operator(t_token *token_last, void **head);

t_node_type	get_cmd(t_token *token_first, void **head, t_cmd *redirects)
{
	int	word_count;
	int	i;

	i = 0;
	if (!redirects)
		return (ERROR);
	word_count = count_words(token_first);
	if (word_count == -1)
	{
		cmd_free(redirects);
		return (ERROR);
	}
	redirects->args =  ft_calloc(word_count + 1, sizeof(char *));
	if (!redirects->args)
		return(err_pars("malloc", redirects, &token_first));
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
	t_token			*token_last;
	t_cmd			*redirects;
	t_redir			*node;
	int				output;

	if (!token_first)
		return (ERROR);
	output = redirects_get(&token_first, &redirects);
	if (output == -1)
		return (ERROR);
	if (token_first && token_first->type == T_LPAREN)
	{
		node = new_redir_node();
		if (!node)
			return(err_pars("malloc", redirects, &token_first));
		token_last = skip_parens(token_first, 1);
		if (!token_last)
		{
			token_delete_all(&token_first);
			free(node);
			cmd_free(redirects);
			return (ERROR);
		}
		if (token_last->next)
		{
			print_syntax_err(token_last->next);
			free(node);
			cmd_free(redirects);
			return (ERROR);
		}
		token_last = token_last->prev;
		token_delete(&token_last->next);
		token_delete(&token_first);
		if (!token_first)
		{
			print_syntax_err(NULL);
			free(node);
			cmd_free(redirects);
			return (ERROR);
		}
		node->redirects = redirects;
		node->type = split_by_operator(token_last, &node->next);
		if (node->type == ERROR)
		{
			cmd_free(redirects);
			free(node);
			return (ERROR);
		}
		*head = node;
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
			return(err_pars("malloc", NULL, &token_first));
		if (!token_last->next || !token_last->prev)
		{
			print_syntax_err(token_last);
			free(node);
			return (ERROR);
		}
		token_delete(&token_last);
		token_split(token_last, -1);
		node->type_left = check_parens(token_first, &node->left);
		if (node->type_left == ERROR)
		{
			token_delete_all(&token_last);
			free(node);
			return (ERROR);
		}
		node->type_right = split_by_pipe(token_last, &node->right);
		if (node->type_right == ERROR)
		{
			free(node);
			return (ERROR);
		}
		*head = node;
		return (PIPE);
	}
	return (check_parens(token_first, head));
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
		node = new_node();
		if (!node)
			return(err_pars("malloc", NULL, &token_first));
		if (token_first->type == T_AND)
			return_value = AND;
		else
			return_value = OR;
		if (!token_first->prev || !token_first->next)
		{
			print_syntax_err(token_first);
			free(node);
			return (ERROR);
		}
		token_delete(&token_first);
		node->type_left = split_by_operator(token_split(token_first, -1),
				&node->left);
		if (node->type_left == ERROR)
		{
			free(node);
			return (ERROR);
		}
		node->type_right = split_by_pipe(token_first, &node->right);
		if (node->type_left == ERROR)
		{
			free(node);
			return (ERROR);
		}
		*head = node;
		return (return_value);
	}
	return (split_by_pipe(token_first, head));
}

t_node_type	tokens_to_tree(t_token *token_last, void **node_tree)
{
	//call split_by_operator directly
	t_node_type	node_type;

	node_type = split_by_operator(token_last, node_tree);
	return (node_type);
}
