/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 00:00:08 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

t_token	*test(t_token *tokens, char **redirect, bool *boolean, bool value)
{
	*boolean = value;
	token_delete(&tokens);
	if (!tokens || tokens->type != T_WORD)
		return NULL; //error
	*redirect = tokens->value;
	tokens->value = NULL;
	token_delete(&tokens);
	return tokens;
}

void	redirects_fill(t_token *tokens, t_cmd *redirects)
{
	int	in_count;
	int	out_count;

	in_count = redirect_get_length(redirects->redirect_in);
	out_count = redirect_get_length(redirects->redirect_out);
	while (tokens)
	{
		if (tokens->type == T_LPAREN)
			tokens = (skip_parens(tokens, 1))->next;
		else if (tokens->type == T_REDIR_IN && ++in_count)
			tokens = test(tokens, &(redirects->redirect_in[in_count]),
				&(redirects->heredoc[in_count]), 0);
		else if (tokens->type == T_REDIR_HEREDOC && ++in_count)
			tokens = test(tokens, &(redirects->redirect_in[in_count]),
				&(redirects->heredoc[in_count]), 1);
		else if (tokens->type == T_REDIR_OUT && ++out_count)
			tokens = test(tokens, &(redirects->redirect_out[out_count]),
				&(redirects->append[out_count]), 0);
		else if (tokens->type == T_REDIR_APPEND && ++out_count)
			tokens = test(tokens, &(redirects->redirect_out[out_count]),
				&(redirects->append[out_count]), 1);
		else
			(void)in_count; //error
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
