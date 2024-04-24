/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:48:01 by padam             #+#    #+#             */
/*   Updated: 2024/04/25 01:39:11 by antonweizma      ###   ########.fr       */
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

char	**word_split(char const *s, char *c, char quotes)
{
	static int	iteration = 0;
	int			lettercount;
	char		**split_words;

	lettercount = 0;
	while (*s && ft_strchr(c, *s))
		s++;
	while (s[lettercount] && (!ft_strchr(c, s[lettercount]) || quotes))
	{
		if (s[lettercount] == quotes)
			quotes = 0;
		else if (!quotes && (s[lettercount] == '"' || s[lettercount] == '\''))
			quotes = s[lettercount];
		lettercount++;
	}
	if (lettercount > 0 && ++iteration)
	{
		split_words = word_split(s + lettercount, c, quotes);
		if (iteration-- && !split_words)
			return (NULL);
		split_words[iteration] = ft_substr(s, 0, lettercount);
		lettercount = 1;
		if (!split_words[iteration])
			while (split_words[iteration + lettercount])
				free(split_words[iteration + lettercount++]);
		if (!split_words[iteration])
			return (free(split_words), NULL);
	}
	else
		split_words = ft_calloc((iteration + 1), sizeof(char *));
	return (split_words);
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

char	*word_join(char **args)
{
	int		i;
	int		l;
	char	*output;

	i = 0;
	l = 0;
	l = 0;
	while (args && args[i])
		l += ft_strlen(args[i++]) + 1;
	output = ft_calloc(l + 1, sizeof(char));
	if (!output) //free
		return (NULL);
	i = 0;
	while (args && args[i])
	{
		ft_strlcat(output, args[i], l);
		free(args[i]);
		ft_strlcat(output, " ", l);
		i++;
	}
	free(args);
	return (output);
}

char **word_splitting(char **args)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = word_join(args);
	args = word_split(tmp, " \t\n", 0);
	free(tmp);
	while (args && args[i])
	{
		args[i] = expander(args[i]);
		i++;
	}
	return (args);
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
		j++;
	}
	args = word_splitting(args);
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
