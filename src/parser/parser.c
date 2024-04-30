/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 02:15:17 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "execution.h"

int	check_quotes(char *command)
{
	char	quote;
	int		paren;

	paren = 0;
	while (*command && paren >= 0)
	{
		if (*command == '(')
			paren++;
		if (*command == ')')
			paren--;
		if (*command == '"' || *command == '\'')
		{
			quote = *command;
			command++;
			while (*command && *command != quote)
				command++;
			if (!*command)
				return (-1 - !isatty(STDIN_FILENO));
		}
		if (paren < 0)
			return (-2);
		command++;
	}
	return ((-1 - !isatty(STDIN_FILENO)) * (paren > 0));
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
	char	*folder;
	char	*command;
	char	*line;

	(void)env;
	if (isatty(STDIN_FILENO) && !DEBUG) //debug
	{
		folder = get_current_folder();
		prompt_tmp = get_env(env, "USER");
		prompt = ft_strjoin(prompt_tmp, "@minishell:");
		free(prompt_tmp);
		// prompt_tmp = ft_strjoin(prompt, RED);
		prompt_tmp = ft_strjoin_free(prompt, folder);
		// prompt_tmp = ft_strjoin(prompt, RESET);
		prompt = ft_strjoin(prompt_tmp, "$ ");
		free(prompt_tmp);
		rl_on_new_line();
		command = readline(prompt);
		free(prompt);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (NULL);
		command = ft_strtrim(line, "\n");
		free(line);
	}
	if (command && *command && *command != '\n' && isatty(STDIN_FILENO) && !DEBUG) //debug
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
			exit_shell(exec, NULL, exec->exit_status);
	}
	while (check_quotes(command) == -1)
	{
		tmp = ft_strjoin(command, "\n");
		free(command);
		command = ft_strjoin_free(tmp, readline("> "));
		if (check_quotes(command) != -1)
			add_history(command);
	}
	if (check_quotes(command) == -2)
	{
		ft_putstr_fd("minishell: unclosed parenthesis\n", 2);
		exec->exit_status = 2;
		free(command);
		return (ERROR);
	}
	tokens = get_next_token(command, NULL);
	free(command);
	type_first = tokens_to_tree(tokens, token_tree);
	climb_tree(*token_tree, type_first);
	if (type_first == ERROR)
		exec->exit_status = 2;
	command = NULL;
	return (type_first);
}
