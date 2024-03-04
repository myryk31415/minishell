/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:11:07 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 20:48:48 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 * @brief removes one token from linked list
*/
void	token_delete(t_token **token)
{
	t_token	*tmp;

	if ((!*token))
		return ;
	free((*token)->value);
	tmp = *token;
	if ((*token)->prev)
		(*token)->prev->next = (*token)->next;
	if ((*token)->next)
		(*token)->next->prev = (*token)->prev;
	*token = (*token)->next;
	free(tmp);
}

/*
 * @brief adds a token to the end of the linked list
*/
t_token	*token_add(t_token *token_last, t_token_type token_type)
{
	t_token	*new_token;

	if (token_last && token_last->type == T_WORD && token_type == T_WORD)
		return (token_last);
	if (token_last && token_last->type == T_SEPARATOR)
	{
		token_last->type = token_type;
		return (token_last);
	}
	new_token = malloc(sizeof(t_token));
	new_token->next = NULL;
	new_token->prev = token_last;
	new_token->type = token_type;
	new_token->value = NULL;
	if (token_last)
		token_last->next = new_token;
	return (new_token);
}

/*
 * @brief splits a token linked list in two parts
 * @param tokens the token after which the list is split
 * @param direction "1" for forward, "-1" for backward
 * @return first token of new list
*/
t_token	*token_split(t_token *tokens, int direction)
{
	t_token	*tmp;

	if (!tokens)
		return (NULL);
	if (direction == 1)
	{
		tmp = tokens->next;
		tokens->next = NULL;
		if (tmp)
			tmp->prev = NULL;
		return (tmp);
	}
	tmp = tokens->prev;
	tokens->prev = NULL;
	if (tmp)
		tmp->next = NULL;
	return (tmp);
}

/*
 * @brief skips the tokens inside parenthesis
 * @param tokens the opening parenthesis token
 * @param direction "1" for forward, "-1" for backward
 * @return the closing parenthesis token
*/
t_token *skip_parens(t_token *tokens, int direction)
{
	int level;

	level = 1;
	while (tokens && level > 0)
	{
		if (direction == 1)
			tokens = tokens->next;
		else
			tokens = tokens->prev;
		if (tokens->type == T_LPAREN)
			level += direction;
		else if (tokens->type == T_RPAREN)
			level -= direction;
	}
	return (tokens);
}
