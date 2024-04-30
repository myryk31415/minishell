/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:14:18 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 17:18:30 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_quotes(int lettercount, char *c, char const *s, char *quotes)
{
	while (s[lettercount] && (!ft_strchr(c, s[lettercount]) || *quotes))
	{
		if (s[lettercount] == *quotes)
			*quotes = 0;
		else if (!*quotes && (s[lettercount] == '"' || s[lettercount] == '\''))
			*quotes = s[lettercount];
		lettercount++;
	}
	return (lettercount);
}

char	**word_split(char const *s, char *c, char quotes)
{
	static int	iteration = 0;
	int			lettercount;
	char		**split_words;

	lettercount = 0;
	while (*s && ft_strchr(c, *s))
		s++;
	lettercount = split_quotes(lettercount, c, s, &quotes);
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
	if (!output)
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

char	**word_splitting(char **args)
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
