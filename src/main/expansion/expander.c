/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:48:01 by padam             #+#    #+#             */
/*   Updated: 2024/04/24 12:37:55 by padam            ###   ########.fr       */
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

char *expander(char *arg)
{
	char *output;
	char *tmp;
	int i;

	tmp = arg;
	output = NULL;
	while (arg && *arg)
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
			output = extract(output, ft_substr(arg, 0, i));
			i++;
		}
		else
		{
			while (arg[i] && arg[i] != '\'' && arg[i] != '"')
				i++;
			output = extract(output, ft_substr(arg, 0, i));
		}
		arg += i;
	}
	free(tmp);
	return (output);
}

char 	**expander_array(char **args, t_exec *exec)
{
	int		i;
	int		j;
	int		k;
	char 	**new_args;

	j = 0;
	new_args = NULL;
	while (args[j])
	{
		args[j] = expand_variables(args[j], exec);
		args[j] = expander(args[j]);
		j++;
	}
	i = 0;
	k = 0;
	while (k < j)
		if (!args[k++])
			i++;
	if (i)
	{
		new_args = ft_calloc(j - i + 1, sizeof(char *));
		k = 0;
		i = 0;
		while (k <= j)
		{
			if (!args[k])
				k++;
			else
				new_args[i++] = ft_strdup(args[k++]);
		}
		free_str_array(args);
		return (new_args);
	}
	return (args);
}

int	heredoc_expand(int heredoc, t_exec *exec)
{
	char *tmp;
	int		fd[2];

	if (pipe(fd) == -1)
		return(perror("minishell: Pipe:"), -1);
	while (1)
	{
		tmp = get_next_line(heredoc);
		if (!tmp)
			break ;
		tmp = expand_variables(tmp, exec);
		write(fd[1], tmp, ft_strlen(tmp));
			free(tmp);
	}
	close(heredoc);
	close(fd[1]);
	return (fd[0]);
}
