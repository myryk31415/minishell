/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:02:47 by padam             #+#    #+#             */
/*   Updated: 2024/03/05 01:28:53 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	count_words(t_token *token_first)
{
	int	word_count;

	word_count = 0;
	while (token_first)
	{
		if (token_first->type != T_WORD)
			return(-1);
		word_count++;
		token_first = token_first->next;
	}
	return (word_count);
}

t_token	*get_pipe(t_token *token_first)
{
	//token_deletion
	while (token_first)
	{
		if (token_first->type == T_LPAREN)
			token_first = skip_parens(token_first, 1);
		else if (token_first->type == T_PIPE)
			return (token_first);
		if (!token_first->next)
			return (token_first);
		token_first = token_first->next;
	}
	// if (token_first->type == T_RPAREN)
	// 	token_first = token_first->prev;
	return (token_first);
}

t_token	*get_operator(t_token *tokens)
{
	//token_deletion
	while (tokens)
	{
		if (tokens->type == T_RPAREN)
			tokens = skip_parens(tokens, -1);
		else if (tokens->type == T_AND || tokens->type == T_OR)
			return (tokens);
		if (!tokens->prev)
			return (tokens);
		tokens = tokens->prev;
	}
	// if (tokens->type == T_LPAREN)
	// 	tokens = tokens->next;
	return (tokens);
}
