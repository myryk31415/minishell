/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/02/19 15:18:18 by padam            ###   ########.fr       */
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

	while (1)
	{
		command = new_prompt();
		// if (ft_strncmp(command, "exit", 4) == 0)
		// 	break ;
		// ft_printf("%s\n", command);
		tokenize_command(command);
		free(command);
	}
}
