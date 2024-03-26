/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/03/26 03:06:32 by antonweizma      ###   ########.fr       */
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
	char	*prompt_tmp;
	char	*command;

	prompt = ft_strjoin(getenv("USER"), "@minishell:");
	prompt_tmp = ft_strjoin(prompt, CYAN);
	free(prompt);
	prompt = ft_strjoin(prompt_tmp, get_current_folder());
	free(prompt_tmp);
	prompt_tmp = ft_strjoin(prompt, RESET);
	free(prompt);
	prompt = ft_strjoin(prompt_tmp, "$ ");
	free(prompt_tmp);
	// ft_putstr_fd(prompt, 0);
	command = readline(prompt);
	// command = get_next_line(0);
	free(prompt);
	if (command)
		add_history(command);
	return (command);
}


t_node_type	parser(void **token_tree, int exit_status)
{
	char		*command;
	t_token		*tokens;
	t_node_type	type_first;

		command = NULL;
	(void)type_first;
	while (!command || !*command)
		command = new_prompt();
	command = expand_variables(command, exit_status);
	tokens = tokenize_command(command);
	free(command);
	type_first = tokens_to_tree(tokens, token_tree);
	if (type_first == ERROR)
		printf("syntax error\n");
	// if (*token_tree)
	// 	get_next_debug(*token_tree, type_first, 0);
	// debug_print_token_array(tokens);
	command = NULL;
	return (type_first);
}
