/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:13:35 by padam             #+#    #+#             */
/*   Updated: 2024/04/11 14:59:05 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_variable(char **command, t_exec *exec)
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
		variable = ft_itoa(exec->exit_status);
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
		variable = get_env(*exec->env, tmp);
		free(tmp);
		(*command) += i;
	}
	return (variable);
}

char	*get_expansion(char *command, int len, t_exec *exec, int not_empty)
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
		variable = get_variable(&tmp, exec);
		if (variable || i)
			not_empty = 1;
		tmp = get_expansion(tmp, len + i + ft_strlen(variable), exec, not_empty);
		ft_memcpy(tmp + len + i, variable, ft_strlen(variable));
		free(variable);
	}
	else
	{
		if (i)
			not_empty = 1;
		if (!not_empty)
			return (NULL);
		tmp = ft_calloc(len + i + 1, sizeof(char));
	}
	ft_memcpy(tmp + len, command, i);
	return (tmp);
}

void	expand_variables(char **command, t_exec *exec)
{
	char	*tmp;

	while (*command)
	{
		tmp = *command;
		*command = get_expansion(*command, 0, exec, 0);
		free(tmp);
		command++;
	}
}

// char	*expand_variables(char *command, t_exec *exec)
// {
// 	char	*tmp;

// 	tmp = command;
// 	command = get_expansion(command, 0, exec, 0);
// 	free(tmp);
// 	return (command);
// }