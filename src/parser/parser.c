/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/04/25 12:54:20 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_quotes(char *command)
{
	char	quote;

	while (*command)
	{
		if (*command == '"' || *command == '\'')
		{
			quote = *command;
			command++;
			while (*command && *command != quote)
				command++;
			if (!*command)
			{
				// ft_putstr_fd("minishell: unclosed quote\n", 2);
				return (-1);
			}
		}
		command++;
	}
	return (0);
}

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

char	*new_prompt(char **env)
{
	char	*prompt;
	char	*prompt_tmp;
	char	*command;
	char	*folder;
	char	*line;

	folder = get_current_folder();
	prompt_tmp = get_env(env, "USER");
	prompt = ft_strjoin(prompt_tmp, "@minishell:");
	free(prompt_tmp);
	prompt_tmp = ft_strjoin(prompt, CYAN);
	free(prompt);
	prompt = ft_strjoin(prompt_tmp, folder);
	free(folder);
	free(prompt_tmp);
	prompt_tmp = ft_strjoin(prompt, RESET);
	free(prompt);
	prompt = ft_strjoin(prompt_tmp, "$ ");
	free(prompt_tmp);
	if (isatty(STDIN_FILENO)) //debug
		command = readline(prompt);
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (NULL);
		command = ft_strtrim(line, "\n");
		free(line);
	}
	// ft_putstr_fd(prompt, 0);
	free(prompt);
	if (command && *command && *command != '\n')
		add_history(command);
	return (command);
}


t_node_type	parser(void **token_tree, t_exec *exec)
{
	char		*command;
	t_token		*tokens;
	t_node_type	type_first;
	char		*tmp;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	command = NULL;
	while (!command || !*command || *command == '\n')
	{
		free(command);
		command = new_prompt(*exec->env);
		if (!command) //need to free
		{
			free_env(exec->env);
			exit(exec->exit_status);
			free(exec);
		}
	}
	while (check_quotes(command) == -1)
	{
		tmp = ft_strjoin(command, "\n");
		free(command);
		command = ft_strjoin(tmp, readline("> "));
		free(tmp);
		if (check_quotes(command) != -1)
			add_history(command);
	}
	tokens = get_next_token(command, NULL);
	free(command);
	type_first = tokens_to_tree(tokens, token_tree);
	climb_tree(*token_tree, type_first);
	// if (type_first == ERROR)
	// 	ft_putstr_fd("syntax error\n", 2);
	if (type_first == ERROR)
		exec->exit_status = 2;
	command = NULL;
	return (type_first);
}
