/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:50 by padam             #+#    #+#             */
/*   Updated: 2024/02/16 18:02:09 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	add_token(t_token **token_last, t_token *new_token)
{
	if (!*token_last)
		*token_last = new_token;
	else
	{
		(*token_last)->next = new_token;
		new_token->prev = *token_last;
		*token_last = new_token;
	}
}

void	tokenize_command(char *command)
{
	t_token	*token_first;
	t_token	*token_last;

	while (*command)
	{

	}
}