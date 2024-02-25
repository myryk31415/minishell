/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:48:38 by padam             #+#    #+#             */
/*   Updated: 2024/02/24 12:03:26 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/*
 * @brief removes one token from linked list
*/
void	delete_token(t_token **token)
{
	t_token	*tmp;

	free((*token)->value);
	tmp = *token;
	*token = (*token)->next;
	free(tmp);
}

/*
 * @brief splits a token linked list in two parts
 * @param tokens the token after which the list is split
 * @return first token of new list
*/
t_token	*split_tokens(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens->next;
	tokens->next = NULL;
	return (tmp);
}
