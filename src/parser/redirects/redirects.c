/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/02/27 17:08:28 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	redirects_count(t_token *tokens, int *in_count, int *out_count)
{
	*in_count = 0;
	*out_count = 0;
	while (tokens)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens);
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

void	redirect_realloc(char ***redirects, bool **boolean, int count)
{
	char	**new_redirects;
	bool	*new_boolean;
	int		i;

	i = 0;
	while (redirects[i])
		i++;
	new_redirects = ft_calloc(i + count + 1, sizeof(char *));
	if (!new_redirects)
		return ;
	new_redirects[i + count] = NULL;
	new_boolean = ft_calloc(i + count, sizeof(bool));
	if (!new_boolean)
	{
		free(new_redirects);
		return ;
	}
	while (i--)
	{
		new_redirects[i] = (*redirects)[i];
		new_boolean[i] = (*boolean)[i];
	}
	free(*redirects);
	free(*boolean);
	*boolean = new_boolean;
	*redirects = new_redirects;
}

void	redirects_fill(t_token *tokens, t_cmd *redirects)
{
	int	in_count;
	int	out_count;

	in_count = 0;
	out_count = 0;
	while (tokens && tokens->type != T_AND && tokens->type != T_OR
		&& tokens->type != T_PIPE)
	{
		if (tokens->type == T_LPAREN)
			tokens = skip_parens(tokens);
		else if (tokens->type == T_REDIR_IN
			|| tokens->type == T_REDIR_HEREDOC)
		{
			redirects->heredoc[in_count] = 0;
			if (tokens->type == T_REDIR_HEREDOC)
				redirects->heredoc[in_count] = 1;
			tokens = tokens->next;
			redirects->redirect_in[in_count++] = tokens->value;
			tokens->value = NULL;
		}
		else if (tokens->type == T_REDIR_OUT
			|| tokens->type == T_REDIR_APPEND)
		{
			redirects->append[out_count] = 0;
			if (tokens->type == T_REDIR_APPEND)
				redirects->append[out_count] = 1;
			tokens = tokens->next;
			//if token != T_WORD, error
			redirects->redirect_out[out_count++] = tokens->value;
			tokens->value = NULL;
		}
		if (tokens)
			tokens = tokens->next;
	}
}

t_cmd	*redirects_get(t_token *tokens, t_cmd *redirects)
{
	int	in_count;
	int	out_count;

	redirects = redirects_dup(redirects);
	redirects_count(tokens, &in_count, &out_count);
	if (in_count)
		redirect_realloc(&(redirects->redirect_in), &redirects->heredoc, in_count);
	if (out_count)
		redirect_realloc(&(redirects->redirect_out), &redirects->append, out_count);
	redirects_fill(tokens, redirects);
	return (redirects);
}
