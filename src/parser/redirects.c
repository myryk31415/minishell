/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/03/21 22:33:05 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <errno.h>

int	here_doc(char *limiter)
{
	char	*str;
	int		fd[2];

	if (pipe(fd) == -1)
		return(perror("minishell: Pipe:"), -1);
	while (1)
	{
		str = readline(">");
		if (str && *str)
		{
			if (!ft_strncmp(str, limiter, ft_strlen(limiter))
				&& !ft_strncmp(str, limiter, ft_strlen(str) - 1))
			{
				free(str);
				break ;
			}
			write(fd[1], str, ft_strlen(str));
			free(str);
		}
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	return (fd[0]);
}

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
	if (value)
		redirects->heredoc[*count] = here_doc((*token_first)->value);
	else
	{
		redirects->heredoc[*count] = value;
		redirects->redirect_in[*count] = (*token_first)->value;
		(*token_first)->value = NULL;
	}
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
	redirects->heredoc = ft_calloc(in_count, sizeof(bool));
	if (!redirects->heredoc)
		return (-1);
	redirects->redirect_out = ft_calloc(out_count + 1, sizeof(char *));
	if (!redirects->redirect_out)
		return (-1);
	redirects->append = ft_calloc(out_count, sizeof(bool));
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

// t_token	*test(t_token *tokens, char **redirect, int *boolean, bool value)
// {
// 	*boolean = value;
// 	token_delete(&tokens);
// 	if (!tokens || tokens->type != T_WORD)
// 		return (NULL); //error
// 	*redirect = tokens->value;
// 	token_delete(&tokens);
// 	return (tokens);
// }

// int	redirect_realloc(char **redirects, int **boolean_new, int count)
// {
// 	char	**redirects_new;
// 	int		i;

// 	i = 0;
// 	if (redirects_old)
// 		while (redirects_old[i])
// 			i++;
// 	redirects_new = ft_calloc(i + count + 1, sizeof(char *));
// 	if (!redirects_new)
// 		return (NULL);
// 	*boolean_new = ft_calloc(i + count, sizeof(bool));
// 	if (!(*boolean_new))
// 	{
// 		free(redirects_new);
// 		return (NULL);
// 	}
// 	while (i--)
// 	{
// 		redirects_new[i] = ft_strdup(redirects_old[i]);
// 		(*boolean_new)[i] = boolean_old[i];
// 	}
// 	return (redirects_new);
// }

// int	get_input(t_token **token_first, t_cmd *redirects, bool value)
// {
// 	token_delete(token_first);
// 	if (!(*token_first) || (*token_first)->type != T_WORD)
// 	{
// 		print_syntax_err(*token_first);
// 		return (-2);
// 	}
// 	if (redirects->redirect_in)
// 		close(redirects->redirect_in);
// 	if (value)
// 		redirects->redirect_in = here_doc((*token_first)->value);
// 	else
// 		redirects->redirect_in = open((*token_first)->value, O_RDONLY, 0666);
// 	if (redirects->redirect_in == -1)
// 	{
// 		err_pars((*token_first)->value, NULL, token_first);
// 		return (-1);
// 	}
// 	token_delete(token_first);
// 	return (0);
// }

// int	get_output(t_token **token_first, t_cmd *redirects, bool value)
// {
// 	token_delete(token_first);
// 	if (!(*token_first) || (*token_first)->type != T_WORD)
// 	{
// 		print_syntax_err(*token_first);
// 		return (-2);
// 	}
// 	if (redirects->redirect_out)
// 		close(redirects->redirect_out);
// 	if (value)
// 		redirects->redirect_out = open((*token_first)->value,
// 				O_WRONLY | O_APPEND | O_CREAT, 0666);
// 	else
// 		redirects->redirect_out = open((*token_first)->value,
// 				O_WRONLY | O_TRUNC | O_CREAT, 0666);
// 	if (redirects->redirect_out == -1)
// 	{
// 		err_pars((*token_first)->value, NULL, token_first);
// 		return (-1);
// 	}
// 	token_delete(token_first);
// 	return (0);
// }

// int	get_fds(t_token **token_first, t_cmd *new_redirects)
// {
// 	int	output;

// 	output = 0;
// 	while (*token_first)
// 	{
// 		if ((*token_first)->type == T_LPAREN)
// 		{
// 			token_first = &skip_parens(*token_first, 1)->next; //brackets?
// 			if (!*token_first)
// 				return (-2);
// 		}
// 		else if ((*token_first)->type == T_REDIR_IN)
// 			output = get_input(token_first, new_redirects, false);
// 		else if ((*token_first)->type == T_REDIR_HEREDOC)
// 			output = get_input(token_first, new_redirects, true);
// 		else if ((*token_first)->type == T_REDIR_OUT)
// 			output = get_output(token_first, new_redirects, false);
// 		else if ((*token_first)->type == T_REDIR_APPEND)
// 			output = get_output(token_first, new_redirects, true);
// 		else
// 			token_first = &((*token_first)->next);
// 		if (output < 0)
// 			return (output);
// 	}
// 	return (0);
// }

// int	redirects_get(t_token **token_first, t_cmd **redirects)
// {
// 	int		output;

// 	//token return
// 	*redirects = ft_calloc(1, sizeof(t_cmd));
// 	if (!*redirects)
// 		return (-1);
// 	(*redirects)->args = NULL;
// 	(*redirects)->redirect_in = 0;
// 	(*redirects)->redirect_out = 0;
// 	output = get_fds(token_first, *redirects);
// 	if (output < 0)
// 	{
// 		free(*redirects);
// 		*redirects = NULL;
// 	}
// 	return (output);
// }
