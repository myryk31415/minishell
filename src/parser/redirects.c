/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/03/13 15:45:40 by padam            ###   ########.fr       */
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
	}
	close(fd[1]);
	return (fd[0]);
}

int	get_input(t_token **token_first, t_cmd *redirects, bool value)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
	{
		ft_printf("minishell: syntax error near unexpected token `%s'\n",
			(*token_first)->value);
		token_delete_all(token_first);
		return (-2);
	}
	if (redirects->redirect_in)
		close(redirects->redirect_in);
	if (value)
		redirects->redirect_in = here_doc((*token_first)->value);
	else
		redirects->redirect_in = open((*token_first)->value, O_RDONLY, 0666);
	if (redirects->redirect_in == -1)
	{
		print_err((*token_first)->value);
		token_delete_all(token_first);
		return (-1);
	}
	token_delete(token_first);
	return (0);
}

int	get_output(t_token **token_first, t_cmd *redirects, bool value)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
	{
		ft_printf("minishell: syntax error near unexpected token `%s'\n",
			(*token_first)->value);
		token_delete_all(token_first);
		return (-2);
	}
	if (redirects->redirect_out)
		close(redirects->redirect_out);
	if (value)
		redirects->redirect_out = open((*token_first)->value,
				O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		redirects->redirect_out = open((*token_first)->value,
				O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (redirects->redirect_out == -1)
	{
		print_err((*token_first)->value);
		token_delete_all(token_first);
		return (-1);
	}
	token_delete(token_first);
	return (0);
}

int	get_fds(t_token **token_first, t_cmd *new_redirects)
{
	int	output;

	output = 0;
	while (*token_first)
	{
		if ((*token_first)->type == T_LPAREN)
		{
			token_first = &skip_parens(*token_first, 1)->next; //brackets?
			if (!*token_first)
				return (-2);
		}
		else if ((*token_first)->type == T_REDIR_IN)
			output = get_input(token_first, new_redirects, false);
		else if ((*token_first)->type == T_REDIR_HEREDOC)
			output = get_input(token_first, new_redirects, true);
		else if ((*token_first)->type == T_REDIR_OUT)
			output = get_output(token_first, new_redirects, false);
		else if ((*token_first)->type == T_REDIR_APPEND)
			output = get_output(token_first, new_redirects, true);
		else
			token_first = &((*token_first)->next);
		if (output < 0)
			return (output);
	}
	return (0);
}

int	redirects_get(t_token **token_first, t_cmd **redirects)
{
	int		output;

	//token return
	*redirects = ft_calloc(1, sizeof(t_cmd));
	if (!*redirects)
		return (-1);
	(*redirects)->args = NULL;
	(*redirects)->redirect_in = 0;
	(*redirects)->redirect_out = 0;
	output = get_fds(token_first, *redirects);
	if (output < 0)
	{
		free(*redirects);
		*redirects = NULL;
	}
	return (output);
}
