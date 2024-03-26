/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:13:35 by padam             #+#    #+#             */
/*   Updated: 2024/03/26 16:05:01 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_variable(char **command, int exit_status, char **env)
{
	char	*variable;
	char	*tmp;
	int		i;

	i = 0;
	variable = NULL;
	if (!*command)
	{
		print_syntax_err(NULL);
		return (NULL);
	}
	if (**command == '?')
	{
		variable = ft_itoa(exit_status);
		(*command)++;
	}
	else if (!**command || !is_variable(**command))
	{
		if (!**command || !is_quote(**command))
			variable = ft_strdup("$");
	}
	else
	{
		while ((*command)[i] && is_variable((*command)[i]))
			i++;
		tmp = ft_substr(*command, 0, i);
		variable = get_env(env, tmp);
		free(tmp);
		(*command) += i;
	}
	return (variable);
}

char	*get_expansion(char *command, int exit_status, int len, char **env)
{
	char	*tmp;
	int		i;
	char	*variable;

	i = 0;
	while (command[i] && command[i] != '$')
	{
		if (command[i] == '\'')
		{
			i++;
			while (command[i] && command[i] != '\'')
				i++;
		}
		if (command[i])
			i++;
	}
	if (command[i] == '$')
	{
		tmp = &command[i + 1];
		variable = get_variable(&tmp, exit_status, env);
		tmp = get_expansion(tmp, exit_status, len + i + ft_strlen(variable));
		ft_memcpy(tmp + len + i, variable, ft_strlen(variable));
		// free(variable);
	}
	else
		tmp = ft_calloc(len + i + 1, sizeof(char));
	ft_memcpy(tmp + len, command, i);
	return (tmp);
}

char	*expand_variables(char *command, int exit_status, char **env)
{
	char	*tmp;

	tmp = command;
	command = get_expansion(command, exit_status, 0, env);
	free(tmp);
	return (command);
}
