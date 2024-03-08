/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/03/08 12:41:30 by aweizman         ###   ########.fr       */
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
		return (NULL); //error
	*redirect = tokens->value;
	token_delete(&tokens);
	return (tokens);
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
		else if (tokens->type == T_REDIR_IN)
		{
			tokens = test(tokens, &(redirects->redirect_in[in_count]),
					&(redirects->heredoc[in_count]), 0);
			in_count++;
		}
		else if (tokens->type == T_REDIR_HEREDOC)
		{
			tokens = test(tokens, &(redirects->redirect_in[in_count]),
					&(redirects->heredoc[in_count]), 1);
			in_count++;
		}
		else if (tokens->type == T_REDIR_OUT)
		{
			tokens = test(tokens, &(redirects->redirect_out[out_count]),
					&(redirects->append[out_count]), 0);
			out_count++;
		}
		else if (tokens->type == T_REDIR_APPEND)
		{
			tokens = test(tokens, &(redirects->redirect_out[out_count]),
					&(redirects->append[out_count]), 1);
			out_count++;
		}
		else
			tokens = tokens->next;
	}
}

char	**redirect_realloc(char **redirects_old, bool *boolean_old,
			bool **boolean_new, int count)
{
	char	**redirects_new;
	int		i;

	i = 0;
	if (redirects_old)
		while (redirects_old[i])
			i++;
	redirects_new = ft_calloc(i + count + 1, sizeof(char *));
	if (!redirects_new)
		return (NULL);
	*boolean_new = ft_calloc(i + count, sizeof(bool));
	if (!(*boolean_new))
	{
		free(redirects_new);
		return (NULL);
	}
	while (i--)
	{
		redirects_new[i] = ft_strdup(redirects_old[i]);
		(*boolean_new)[i] = boolean_old[i];
	}
	return (redirects_new);
}

t_cmd	*redirects_get(t_token **tokens, t_cmd *redirects)
{
	int		in_count;
	int		out_count;
	t_cmd	*new_redirects;

	//token return
	new_redirects = ft_calloc(1, sizeof(t_cmd));
	if (!new_redirects)
		return (NULL);
	new_redirects->args = NULL;
	redirects_count(*tokens, &in_count, &out_count);
	new_redirects->redirect_in = redirect_realloc((redirects->redirect_in),
		redirects->heredoc, &new_redirects->heredoc, in_count);
	new_redirects->redirect_out = redirect_realloc((redirects->redirect_out),
		redirects->append, &new_redirects->append, out_count);
	redirects_fill(*tokens, new_redirects);
	return (new_redirects);
}
