/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:34:22 by padam             #+#    #+#             */
/*   Updated: 2024/03/21 12:50:29 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>

static int	here_doc(char *limiter)
{
	char	*str;
	int		fd[2];

	if (pipe(fd) == -1)
		return (perror("Pipe"), -1);
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

int	get_input(t_token **token_first, t_cmd *redirects, bool value)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
		return (-1); //error
	if (redirects->redirect_in > 2) //okay so?
		close(redirects->redirect_in);
	if (value)
		redirects->redirect_in = here_doc((*token_first)->value);
	else
		redirects->redirect_in = open((*token_first)->value, O_RDONLY, 0666);
	if (redirects->redirect_in == -1)
		return (perror("error"), -1);
	token_delete(token_first);
	return (0);
}

int	get_output(t_token **token_first, t_cmd *redirects, bool value)
{
	token_delete(token_first);
	if (!(*token_first) || (*token_first)->type != T_WORD)
		return (-1); //error
	if (redirects->redirect_out > 2)//okay so?
		close(redirects->redirect_out);
	if (value)
		redirects->redirect_out = open((*token_first)->value,
				O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		redirects->redirect_out = open((*token_first)->value,
				O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (redirects->redirect_out == -1)
		return (perror("error"), -1);
	token_delete(token_first);
	return (0);
}

int	get_fds(t_token **token_first, t_cmd *new_redirects)
{
	while (*token_first)
	{
		if ((*token_first)->type == T_LPAREN)
			token_first = &(skip_parens(*token_first, 1)->next); //brackets?
		else if ((*token_first)->type == T_REDIR_IN)
		{
			if (get_input(token_first, new_redirects, false) == -1)
				return (-1);
		}
		else if ((*token_first)->type == T_REDIR_HEREDOC)
		{
			if (get_input(token_first, new_redirects, true) == -1)
				return (-1);
		}
		else if ((*token_first)->type == T_REDIR_OUT)
		{
			if (get_output(token_first, new_redirects, false) == -1)
				return (-1);
		}
		else if ((*token_first)->type == T_REDIR_APPEND)
		{
			if (get_output(token_first, new_redirects, true) == -1)
				return (-1);
		}
		else
			token_first = &((*token_first)->next);
	}
	return (0);
}

t_cmd	*redirects_get(t_token **token_first)
{
	t_cmd	*new_redirects;

	//token return
	new_redirects = ft_calloc(1, sizeof(t_cmd));
	if (!new_redirects)
		return (NULL);
	new_redirects->args = NULL;
	new_redirects->redirect_in = 0;
	new_redirects->redirect_out = 0;
	get_fds(token_first, new_redirects);
	return (new_redirects);
}
