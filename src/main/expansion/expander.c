/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:48:01 by padam             #+#    #+#             */
/*   Updated: 2024/04/11 14:55:34 by padam            ###   ########.fr       */
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

//error handling
t_token	*handle_quotes(char **string, t_token *token_last, t_exec *exec)
{
	char	quote;
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	quote = **string;
	(*string)++;
	if (!**string)
		return (NULL);
	while ((*string)[i + 1] && (*string)[i] != quote)
		i++;
	if ((*string)[i] != quote)
		return (NULL);
	if (!token_last || token_last->type != T_WORD)
		token_last = token_add(token_last, T_WORD);
	if (quote == '\'')
	{
		tmp2 = ft_substr(*string, 0, i);
		tmp = token_last->value;
		token_last->value = ft_strjoin(tmp, tmp2);
		free(tmp);
		free(tmp2);
	}
	else
		token_last->value = var_expand(token_last->value, ft_substr(*string, 0, i), 1, exec);
	token_last->quote = 2;
	*string += i;
	return (token_last);
}

char *handle_word(char *str)
{
	
}

char *expander(char *str, t_exec *exec)
{
	char *output;
	char *tmp;
	char *tmp2;
	int i;

	i = 0;
	output = NULL;
	while (str[i])
	{
		if (str[i] == '\'')
			tmp = handle_quotes(&str, output, exec);
		else if (str[i] == '\"')
			tmp = handle_quotes(&str, output, exec);
		else
			tmp = handle_word(str);
		output = tmp2;
		output = ft_strjoin(output, tmp);
		free(tmp);
		free(tmp2);
	}
	output = var_expand(output, str, 0, exec);
	return (output);
}
