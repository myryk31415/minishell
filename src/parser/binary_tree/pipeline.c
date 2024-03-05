/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:02:47 by padam             #+#    #+#             */
/*   Updated: 2024/03/05 01:05:11 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*get_pipe(t_token *tokens)
{
	//token_deletion
	while (tokens)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens, 1);
		else if (tokens->type == T_PIPE)
			return (tokens);
		if (!tokens->next)
			return (tokens);
		tokens = tokens->next;
	}
	if (tokens->type == T_RPAREN)
		tokens = tokens->prev;
	return (tokens);
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
	if (tokens->type == T_LPAREN)
		tokens = tokens->next;
	return (tokens);
}
