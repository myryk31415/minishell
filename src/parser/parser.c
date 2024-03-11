/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/03/12 08:42:58 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	debug_print_tree(t_node *node, int i);
void	debug_print_redir(t_redir *redir, int i);
void	debug_print_cmd(t_cmd *cmd);

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

void	get_next_debug(void *ptr, t_node_type type, int i)
{
	i++;
	if (type == CMD)
		debug_print_cmd(ptr);
	else if (type == ERROR)
		printf("error\n");
	else if (type == REDIR)
		debug_print_redir(ptr, i);
	else
		debug_print_tree(ptr, i);
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
	char *type_list[] = {"ERROR", "REDIR", "AND", "OR", "PIPE", "CMD"};
	printf("%il%b: %s\n", i, node->new_process_left, type_list[node->type_left]);
	printf("%ir%b: %s\n", i, node->new_process_right, type_list[node->type_right]);
	if (node->left)
		get_next_debug(node->left, node->type_left, i);
	if (node->right)
		get_next_debug(node->right, node->type_right, i);
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
		if (token_tree)
			get_next_debug(token_tree, token_tree_first, 0);
		// debug_print_token_array(tokens);
	}
}
