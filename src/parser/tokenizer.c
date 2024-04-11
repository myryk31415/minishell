/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:50 by padam             #+#    #+#             */
/*   Updated: 2024/04/12 00:23:58 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//change to char pointer
t_token_type	get_token_type(char *string)
{
	t_token_type	token;

	if (is_separator(*string))
		token = T_SEPARATOR;
	else if (!ft_strncmp(string, "&&", 2))
		token = T_AND;
	else if (!ft_strncmp(string, "||", 2))
		token = T_OR;
	else if (!ft_strncmp(string, ">>", 2))
		token = T_REDIR_APPEND;
	else if (!ft_strncmp(string, "<<", 2))
		token = T_REDIR_HEREDOC;
	else if (!ft_strncmp(string, "(", 1))
		token = T_LPAREN;
	else if (!ft_strncmp(string, ")", 1))
		token = T_RPAREN;
	else if (!ft_strncmp(string, "|", 1))
		token = T_PIPE;
	else if (!ft_strncmp(string, ">", 1))
		token = T_REDIR_OUT;
	else if (!ft_strncmp(string, "<", 1))
		token = T_REDIR_IN;
	else
		token = T_WORD;
	return (token);
}

t_token	*handle_word(char **string, t_token *token_last)
{
	t_token_type	token_type;
	int				i;
	char			quote;

	i = 0;
	quote = 0;
	token_type = T_WORD;
	while ((token_type == T_WORD || quote) && (*string)[i])
	{
		if (quote && (*string)[i] == quote)
			quote = 0;
		else if ((*string)[i] == '\'' || (*string)[i] == '"')
			quote = (*string)[i];
		token_type = get_token_type((*string) + i++);
	}
	if (token_type != T_WORD)
		i--;
	if (!token_last || token_last->type != T_WORD)
		token_last = token_add(token_last, T_WORD);
	token_last->value = ft_substr(*string, 0, i);
	*string += i - 1;
	return (token_last);
}

t_token	*get_next_token(char *string, t_token *token_last)
{
	t_token_type	token_type;
	int				i;

	i = 1;
	if (is_separator(*string) && !(*(string + 1)))
		return (token_last);
	token_type = get_token_type(string);
	if (is_separator(*string) && is_separator(*(string + 1)))
		return (get_next_token(string + 1, token_last));
	token_last = token_add(token_last, token_type);
	if (token_type == T_WORD)
		token_last = handle_word(&string, token_last);
	else if (token_type == T_AND || token_type == T_OR
		|| token_type == T_REDIR_APPEND || token_type == T_REDIR_HEREDOC)
		i += 1;
	if (string[i])
		token_last = get_next_token(string + i, token_last);
	return (token_last);
}
