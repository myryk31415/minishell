/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:48:01 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 17:16:25 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_expand(char *old_string, char *string, int quotes, t_exec *exec)
{
	char	*output;

	string = expand_variables(string, exec);
	if (!string && !old_string && !quotes)
		return (NULL);
	output = ft_strjoin(old_string, string);
	free(old_string);
	free(string);
	return (output);
}

char	*expander(char *arg)
{
	char	*output;
	char	*tmp;
	int		i;
	char	quote;

	tmp = arg;
	output = NULL;
	while (*arg)
	{
		i = 0;
		quote = 0;
		if (*arg == '\'' || *arg == '"')
			quote = *arg++;
		while (arg[i] && ((arg[i] != '\'' && arg[i] != '"') || \
			(quote && arg[i] != quote)))
			i++;
		output = ft_strjoin_free(output, ft_substr(arg, 0, i));
		if (quote)
			i++;
		arg += i;
	}
	free(tmp);
	return (output);
}

char	**expander_array(char **args, t_exec *exec)
{
	int		j;

	j = 0;
	while (args[j])
	{
		args[j] = expand_tilde(args[j], *(exec->env));
		args[j] = expand_variables(args[j], exec);
		j++;
	}
	args = word_splitting(args);
	return (args);
}

int	heredoc_expand(int heredoc, t_exec *exec)
{
	char	*tmp;
	int		fd[2];

	if (pipe(fd) == -1)
		return (perror("minishell: Pipe:"), -1);
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
