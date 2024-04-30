/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:15:56 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 18:29:29 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "execution.h"

char	*color_string(char *str, char *color)
{
	char	*out;
	int		len;
	char	*tmp;

	tmp = str;
	len = (ft_strlen(color) + ft_strlen(RESET)) * ft_strlen(str) \
			+ ft_strlen(str) + 1;
	out = ft_calloc(len, 1);
	if (!out)
		return (NULL);
	*out = '\0';
	while (*str)
	{
		ft_strlcat(out, color, len);
		out[ft_strlen(out)] = *str;
		out[ft_strlen(out) + 1] = '\0';
		ft_strlcat(out, RESET, len);
		str++;
	}
	free(tmp);
	return (out);
}

char	*craft_prompt(t_exec *exec)
{
	char	*cwd;
	char	*folder;
	char	*prompt;
	char	*prompt_tmp;

	cwd = getcwd(NULL, 0);
	folder = ft_strdup(ft_strrchr(cwd, '/') + 1);
	if (!folder)
		folder = ft_strdup(cwd);
	free(cwd);
	prompt = get_env(*(exec->env), "USER");
	if (exec->exit_status)
		prompt_tmp = ft_strjoin(RED_INCIDATOR, prompt);
	else
		prompt_tmp = ft_strjoin(MAGENTA_INCIDATOR, prompt);
	free(prompt);
	prompt = ft_strjoin(prompt_tmp, "@minishell:");
	free(prompt_tmp);
	prompt_tmp = ft_strjoin_free(prompt, color_string(folder, CYAN));
	prompt = ft_strjoin(prompt_tmp, "$ ");
	free(prompt_tmp);
	return (prompt);
}

char	*new_prompt(t_exec *exec)
{
	char	*prompt;
	char	*command;
	char	*line;

	if (isatty(STDIN_FILENO) && !DEBUG)
	{
		prompt = craft_prompt(exec);
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
	if (command && *command && *command != '\n' && \
		isatty(STDIN_FILENO) && !DEBUG)
		add_history(command);
	return (command);
}

t_node_type	parser(void **token_tree, t_exec *exec)
{
	char		*command;
	t_token		*tokens;
	t_node_type	type_first;

	signal(SIGINT, parser_handler);
	signal(SIGQUIT, parser_handler);
	command = NULL;
	while (!command || !*command || *command == '\n')
	{
		free(command);
		command = new_prompt(exec);
		if (!command)
			exit_shell(exec, NULL, exec->exit_status);
	}
	command = check_unclosed(command, exec);
	if (!command)
		return (ERROR);
	tokens = get_next_token(command, NULL);
	free(command);
	type_first = split_by_operator(tokens, token_tree);
	climb_tree(*token_tree, type_first);
	if (type_first == ERROR)
		exec->exit_status = 2;
	command = NULL;
	return (type_first);
}
