/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:13:35 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 18:15:05 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_variable_env(char **command, char *tmp, int i, t_exec *exec)
{
	char	*variable;

	variable = NULL;
	while ((*command)[i] && is_variable((*command)[i]))
		i++;
	tmp = ft_substr(*command, 0, i);
	variable = get_env(*exec->env, tmp);
	free(tmp);
	(*command) += i;
	return (variable);
}

char	*get_variable(char **command, t_exec *exec, int quotes)
{
	char	*variable;
	char	*tmp;
	int		i;

	i = 0;
	tmp = NULL;
	variable = NULL;
	if (!*command)
	{
		print_syntax_err(NULL);
		return (NULL);
	}
	if (**command == '?')
	{
		variable = ft_itoa(exec->exit_status);
		(*command)++;
	}
	else if (!**command || !is_variable(**command))
	{
		if (!**command || !is_quote(**command) || quotes)
			variable = ft_strdup("$");
	}
	else
		variable = get_variable_env(command, tmp, i, exec);
	return (variable);
}

int	count_quotes(char *command, int i, int *quotes)
{
	while (command && command[i] && command[i] != '$')
	{
		if (command[i] == '"')
			*quotes = !*quotes;
		if (command[i] == '\'' && !*quotes)
		{
			i++;
			while (command[i] && command[i] != '\'')
				i++;
		}
		if (command[i])
			i++;
	}
	return (i);
}

char	*get_expansion(char *command, int len, t_exec *exec, int quotes)
{
	char	*tmp;
	int		i;
	char	*variable;

	i = count_quotes(command, 0, &quotes);
	if (command && command[i] == '$')
	{
		tmp = &command[i + 1];
		variable = get_variable(&tmp, exec, quotes);
		tmp = get_expansion(tmp, len + i + ft_strlen(variable), exec, quotes);
		ft_memcpy(tmp + len + i, variable, ft_strlen(variable));
		free(variable);
	}
	else
		tmp = ft_calloc(len + i + 1, sizeof(char));
	ft_memcpy(tmp + len, command, i);
	return (tmp);
}

char	*expand_variables(char *command, t_exec *exec)
{
	char	*tmp;

	tmp = command;
	command = get_expansion(command, 0, exec, 0);
	free(tmp);
	return (command);
}
