/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:26:09 by padam             #+#    #+#             */
/*   Updated: 2024/03/13 15:30:07 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	debug_print_token_array(t_token *token_first)
{
	while (token_first)
	{
		printf("type: %d, value: %s\n", token_first->type, token_first->value);
		token_first = token_first->next;
	}
}

void	debug_print_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	printf("----------------\n");
	printf("args:\n");
	while (cmd->args && cmd->args[i])
		printf("%s\n", cmd->args[i++]);
	i = 0;
	printf("redirect_in: %d\n", cmd->redirect_in);
	printf("redirect_out: %d\n", cmd->redirect_out);
}

void	debug_print_redir(t_redir *redir, int i)
{
	if (redir->redirects)
		debug_print_cmd(redir->redirects);
	if (redir->next)
		get_next_debug(redir->next, redir->type, i);
}

void	debug_print_tree(t_node *node, int i)
{
	char *type_list[] = {"ERROR", "REDIR", "AND", "OR", "PIPE", "CMD", "SYNTAX"};
	printf("%il: %s\n", i, type_list[node->type_left]);
	printf("%ir: %s\n", i, type_list[node->type_right]);
	if (node->left)
		get_next_debug(node->left, node->type_left, i);
	if (node->right)
		get_next_debug(node->right, node->type_right, i);
}


void	get_next_debug(void *ptr, t_node_type type, int i)
{
	i++;
	if (type == CMD)
		debug_print_cmd(ptr);
	else if (type == ERROR)
		printf("error\n");
	else if (type == SYNTAX)
		printf("syntax\n");
	else if (type == REDIR)
		debug_print_redir(ptr, i);
	else
		debug_print_tree(ptr, i);
}
