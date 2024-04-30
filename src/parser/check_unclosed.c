/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_unclosed.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:27:41 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 18:29:59 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

char	*check_unclosed(char *command, t_exec *exec)
{
	char	*tmp;

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
		return (NULL);
	}
	return (command);
}
