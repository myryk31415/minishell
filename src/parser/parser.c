/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/02/27 20:29:39 by padam            ###   ########.fr       */
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

void	debug_print_cmd(t_cmd *redirects)
{
	int	i;

	i = 0;
	printf("----------------\n");
	printf("args:\n");
	while (redirects->args && redirects->args[i])
		printf("%s\n", redirects->args[i++]);
	i = 0;
	printf("----------------\n");
	printf("redirect_in:\n");
	while (redirects->redirect_in && redirects->redirect_in[i])
	{
		printf("%i: %s\n", i, redirects->redirect_in[i]);
		if (redirects->heredoc[i++] == true)
			printf("%i: heredoc\n", i - 1);
		else
			printf("%i: no heredoc\n", i - 1);
	}
	i = 0;
	printf("----------------\n");
	printf("redirect_out:\n");
	while (redirects->redirect_out && redirects->redirect_out[i])
	{
		printf("%i: %s\n", i, redirects->redirect_out[i]);
		if (redirects->append[i++] == true)
			printf("%i: append\n", i - 1);
		else
			printf("%i: no append\n", i - 1);
	}
}

void	parser(void)
{
	char	*command;
	t_token	*tokens;
	t_cmd	redirects;
	// t_cmd	*new_redirects;
	void	*token_tree;
	t_node_type	token_tree_first;

	redirects.args = NULL;
	redirects.redirect_in = NULL;
	redirects.redirect_out = NULL;
	redirects.heredoc = NULL;
	redirects.append = NULL;
	while (1)
	{
		command = new_prompt();
		tokens = tokenize_command(command);
		free(command);
		token_tree_first = tokens_to_tree(tokens, &token_tree);
		(void)token_tree_first;
		(void)redirects;
		// new_redirects = redirects_get(tokens, &redirects);
		// debug_print_token_array(tokens);
		// debug_print_cmd(new_redirects);
	}
}
