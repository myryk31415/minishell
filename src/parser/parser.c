/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/03/13 16:31:52 by padam            ###   ########.fr       */
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


void	parser(void)
{
	char	*command;
	t_token	*tokens;
	void	*token_tree;
	t_node_type	token_tree_first;

	command = NULL;
	(void)token_tree_first;
	while (1)
	{
		while (!command || !*command)
			command = new_prompt();
		// command = "he && hi || du";
		if (command)
			tokens = tokenize_command(command);
		free(command);
		token_tree_first = tokens_to_tree(tokens, &token_tree);
		if (token_tree_first == SYNTAX)
			printf("syntax error\n");
		if (token_tree)
			get_next_debug(token_tree, token_tree_first, 0);
		// debug_print_token_array(tokens);
		command = NULL;
	}
}
