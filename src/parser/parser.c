/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/03/07 12:51:29 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
	printf("----------------\n");
	printf("redirect_in:\n");
	while (cmd->redirect_in && cmd->redirect_in[i])
	{
		printf("%i: %s, ", i, cmd->redirect_in[i]);
		if (cmd->heredoc[i++] == true)
			printf("heredoc\n");
		else
			printf("no heredoc\n");
	}
	i = 0;
	printf("----------------\n");
	printf("redirect_out:\n");
	while (cmd->redirect_out && cmd->redirect_out[i])
	{
		printf("%i: %s, ", i, cmd->redirect_out[i]);
		if (cmd->append[i++] == true)
			printf("append\n");
		else
			printf("no append\n");
	}
}

void	debug_print_tree(t_node *node, int i)
{
	char *type_list[] = {"ERROR", "AND", "OR", "PIPE", "CMD"};
	printf("%il%b: %s\n", i, node->new_process_left, type_list[node->type_left]);
	printf("%ir%b: %s\n", i, node->new_process_right, type_list[node->type_right]);
	if (node->left)
	{
		if (node->type_left == CMD)
			debug_print_cmd(node->left);
		else
			debug_print_tree(node->left, i + 1);

	}
	if (node->right)
	{
		if (node->type_right == CMD)
			debug_print_cmd(node->right);
		else
			debug_print_tree(node->right, i + 1);
	}
}

void	parser(void)
{
	char	*command;
	t_token	*tokens;

	void	*token_tree;
	t_node_type	token_tree_first;
	while (1)
	{
		command = new_prompt();
		// command = "he && hi || du";
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
	}
}
