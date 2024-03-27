/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:02:47 by padam             #+#    #+#             */
/*   Updated: 2024/03/13 22:27:35 by padam            ###   ########.fr       */
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
		{
			print_syntax_err(token_first);
			return(-1);
		}
		word_count++;
		token_first = token_first->next;
	}
	if (word_count == 0)
	{
		print_syntax_err(NULL);
		return(-1);
	}
	return (word_count);
}

t_token	*get_pipe(t_token *token_first)
{
	while (token_first)
	{
		if (token_first->type == T_LPAREN)
			token_first = skip_parens(token_first, 1);
		else if (token_first->type == T_PIPE)
			return (token_first);
		if (!token_first || !token_first->next)
			break ;
		token_first = token_first->next;
	}
	return (token_first);
}

t_token	*get_operator(t_token *token_last)
{
	while (token_last)
	{
		if (token_last->type == T_RPAREN)
			token_last = skip_parens(token_last, -1);
		else if (token_last->type == T_AND || token_last->type == T_OR)
			return (token_last);
		if (!token_last || !token_last->prev)
			break ;
		token_last = token_last->prev;
	}
	return (token_last);
}
