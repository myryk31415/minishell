/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/03/27 14:56:22 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <errno.h>

void	redirects_count(t_token *tokens, int *in_count, int *out_count)
{
	*in_count = 0;
	*out_count = 0;
	while (tokens && tokens->type != T_AND && tokens->type != T_OR)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens, 1);
		else if (tokens->type == T_REDIR_IN)
			(*in_count)++;
		else if (tokens->type == T_REDIR_HEREDOC)
			(*in_count)++;
		else if (tokens->type == T_REDIR_OUT)
			(*out_count)++;
		else if (tokens->type == T_REDIR_APPEND)
			(*out_count)++;
		tokens = tokens->next;
	}
}

int	redirect_get_length(char **redirects)
{
	int	i;

	i = 0;
	if (!redirects)
		return (0);
	while (redirects[i])
		i++;
	return (i);
}


int	get_input(t_token **token_first, t_cmd *redirects, bool value, int *count)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
	{
		print_syntax_err(*token_first);
		return (-1);
	}
	redirects->heredoc[*count] = value;
	redirects->redirect_in[*count] = (*token_first)->value;
	(*token_first)->value = NULL;
	token_delete(token_first);
	(*count)++;
	return (0);
}

int	get_output(t_token **token_first, t_cmd *redirects, bool value, int *count)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
	{
		print_syntax_err(*token_first);
		return (-1);
	}
	redirects->append[*count] = value;
	redirects->redirect_out[*count] = (*token_first)->value;
	(*token_first)->value = NULL;
	token_delete(token_first);
	(*count)++;
	return (0);
}

int	redirects_fill(t_token **token_first, t_cmd *redirects)
{
	int	in_count;
	int	out_count;
	int	output;

	output = 0;
	in_count = 0;
	out_count = 0;
	while (*token_first)
	{
		if ((*token_first)->type == T_LPAREN)
			token_first = &skip_parens(*token_first, 1)->next;
		else if ((*token_first)->type == T_REDIR_IN)
			output = get_input(token_first, redirects, false, &in_count);
		else if ((*token_first)->type == T_REDIR_HEREDOC)
			output = get_input(token_first, redirects, true, &in_count);
		else if ((*token_first)->type == T_REDIR_OUT)
			output = get_output(token_first, redirects, false, &out_count);
		else if ((*token_first)->type == T_REDIR_APPEND)
			output = get_output(token_first, redirects, true, &out_count);
		else
			token_first = &((*token_first)->next);
		if (output == -1)
			return (-1);
	}
	return (0);
}

int	redirect_alloc(t_token **token_first, t_cmd	*redirects)
{
	int		in_count;
	int		out_count;

	redirects->args = NULL;
	redirects->redirect_in = NULL;
	redirects->heredoc = NULL;
	redirects->redirect_out = NULL;
	redirects->append = NULL;
	redirects_count(*token_first, &in_count, &out_count);
	redirects->redirect_in = ft_calloc(in_count + 1, sizeof(char *));
	if (!redirects->redirect_in)
		return (-1);
	redirects->heredoc = ft_calloc(in_count + 1, sizeof(bool));
	if (!redirects->heredoc)
		return (-1);
	redirects->redirect_out = ft_calloc(out_count + 1, sizeof(char *));
	if (!redirects->redirect_out)
		return (-1);
	redirects->append = ft_calloc(out_count + 1, sizeof(bool));
	if (!redirects->append)
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
