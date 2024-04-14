/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/04/14 21:55:24 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <errno.h>

int	redirects_count(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != T_AND && tokens->type != T_OR)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens, 1);
		else if (tokens->type == T_REDIR_IN)
			count++;
		else if (tokens->type == T_REDIR_HEREDOC)
			count++;
		else if (tokens->type == T_REDIR_OUT)
			count++;
		else if (tokens->type == T_REDIR_APPEND)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	get_output(t_token **token_first, t_cmd *redirects, int value, int *count)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
	{
		print_syntax_err(*token_first);
		return (-1);
	}
	if (!(*token_first)->value)
	{
		ft_printf("minishell: ambiguous redirect\n");
		token_delete_all(token_first);
		return (-1);
	}
	redirects->redirect_type[*count] = value;
	if (value == 3)
		redirects->redirects[*count] += (*token_first)->quote;
	redirects->redirects[*count] = (*token_first)->value;
	(*token_first)->value = NULL;
	token_delete(token_first);
	(*count)++;
	return (0);
}

int	redirects_fill(t_token **token_first, t_cmd *redirects)
{
	int	count;
	int	output;

	output = 0;
	count = 0;
	while (*token_first)
	{
		if ((*token_first)->type == T_LPAREN)
			token_first = &skip_parens(*token_first, 1)->next;
		else if ((*token_first)->type == T_REDIR_OUT)
			output = get_input(token_first, redirects, 0, &count);
		else if ((*token_first)->type == T_REDIR_APPEND)
			output = get_input(token_first, redirects, 1, &count);
		else if ((*token_first)->type == T_REDIR_IN)
			output = get_input(token_first, redirects, 2, &count);
		else if ((*token_first)->type == T_REDIR_HEREDOC)
			output = get_input(token_first, redirects, 3, &count);
		else
			token_first = &((*token_first)->next);
		if (output == -1)
			return (-1);
	}
	return (0);
}

int	redirect_alloc(t_token **token_first, t_cmd	*redirects)
{
	int		count;

	redirects->args = NULL;
	redirects->redirects = NULL;
	redirects->redirect_type = NULL;
	count = redirects_count(*token_first);
	redirects->redirects = ft_calloc(count + 1, sizeof(char *));
	if (!redirects->redirects)
		return (-1);
	redirects->redirect_type = ft_calloc(count + 1, sizeof(int));
	if (!redirects->redirect_type)
		return (-1);
	return (0);
}

int	redirects_get(t_token **token_first, t_cmd **redirects)
{
	int	output;

	if (!*token_first)
	{
		*redirects = NULL;
		return (0);
	}
	*redirects = ft_calloc(1, sizeof(t_cmd));
	if (!*redirects)
		return (-1);
	output = redirect_alloc(token_first, *redirects);
	if (output == -1)
	{
		cmd_free(*redirects);
		*redirects = NULL;
		return (-1);
	}
	output = redirects_fill(token_first, *redirects);
	if (output == -1)
	{
		free(*redirects);
		*redirects = NULL;
		return (-1);
	}
	return (0);
}
