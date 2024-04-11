/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:48:01 by padam             #+#    #+#             */
/*   Updated: 2024/04/12 00:07:04 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_expand(char *old_string, char *string, int quotes, t_exec *exec)
{
	char *output;

	string = expand_variables(string, exec);
	if (!string && !old_string && !quotes)
		return (NULL);
	output = ft_strjoin(old_string, string);
	free(old_string);
	free(string);
	return (output);
}

char *extract(char *old_string, char *string)
{
	char *output;

	output = ft_strjoin(old_string, string);
	free(old_string);
	free(string);
	return (output);
}

char *expander(char *arg, t_exec *exec)
{
	char *output;
	char *tmp;
	int i;

	tmp = arg;
	output = NULL;
	while (*arg)
	{
		i = 0;
		if (*arg == '\'')
		{
			arg++;
			while (arg[i] != '\'')
				i++;
			output = extract(output, ft_substr(arg, 0, i));
			i++;
		}
		else if (*arg == '"')
		{
			arg++;
			while (arg[i] != '"')
				i++;
			output = var_expand(output, ft_substr(arg, 0, i), 1, exec);
			i++;
		}
		else
		{
			while (arg[i] && arg[i] != '\'' && arg[i] != '"')
				i++;
			output = var_expand(output, ft_substr(arg, 0, i), 0, exec);
		}
		arg += i;
	}
	free(tmp);
	return (output);
}

void	expander_array(char **args, t_exec *exec)
{
	while (*args)
	{
		*args = expander(*args, exec);
		args++;
	}
}
