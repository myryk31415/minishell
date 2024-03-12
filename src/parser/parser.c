/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/03/12 08:41:28 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	debug_print_tree(t_node *node, int i);

char	*get_current_folder(void)
{
	char	*cwd;
	char	*folder;

	cwd = getcwd(NULL, 0);
	folder = ft_strdup(ft_strrchr(cwd, '/') + 1);
	if (!folder)
		folder = ft_strdup(cwd);
	free(cwd);
	return (folder);
}

char	*new_prompt(void)
{
	char	*prompt;
	char	*command;

	prompt = ft_strjoin(getenv("USER"), "@minishell:");
	prompt = ft_strjoin(prompt, CYAN);
	prompt = ft_strjoin(prompt, get_current_folder());
	prompt = ft_strjoin(prompt, RESET);
	prompt = ft_strjoin(prompt, "$ ");
	command = readline(prompt);
	free(prompt);
	if (command)
		add_history(command);
	return (command);
}

// void	debug_print_token_array(t_token *token_first)
// {
// 	while (token_first)
// 	{
// 		printf("type: %d, value: %s\n", token_first->type, token_first->value);
// 		token_first = token_first->next;
// 	}
// }

// void	debug_print_cmd(t_cmd *cmd)
// {
// 	int	i;

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
		debug_print_tree(redir->next, i + 1);
}

void	debug_print_tree(t_node *node, int i)
{
	char *type_list[] = {"ERROR", "REDIR", "AND", "OR", "PIPE", "CMD"};
	printf("%il%b: %s\n", i, node->new_process_left, type_list[node->type_left]);
	printf("%ir%b: %s\n", i, node->new_process_right, type_list[node->type_right]);
	if (node->left)
	{
		if (node->type_left == CMD)
			debug_print_cmd(node->left);
		else if (node->type_left == REDIR)
			debug_print_redir(node->left, i + 1);
		else
			debug_print_tree(node->left, i + 1);

	}
	if (node->right)
	{
		if (node->type_right == CMD)
			debug_print_cmd(node->right);
		else if (node->type_right == REDIR)
			debug_print_redir(node->right, i + 1);
		else
			debug_print_tree(node->right, i + 1);
	}
}

t_node_type	parser(void **token_tree)
{
	char	*command;
	t_token	*tokens;
	t_node_type	token_tree_first;

	(void)token_tree_first;
	while (1)
	{
		command = new_prompt();
		// command = "he && hi || du";
		if (command)
			tokens = tokenize_command(command);
		free(command);
		token_tree_first = tokens_to_tree(tokens, &token_tree);
		if (token_tree_first == ERROR)
			printf("error\n");
		else if (token_tree_first == CMD)
			debug_print_cmd(token_tree);
		else
			debug_print_tree(token_tree, 0);

	// debug_print_token_array(tokens);
	return (token_tree_first);
}
