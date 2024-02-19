/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:50 by padam             #+#    #+#             */
/*   Updated: 2024/02/19 13:05:20 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	add_token(t_token **token_last, t_token_type token_type)
{
	t_token	*new_token;

	if (*token_last && (*token_last)->type == T_NEW)
		return ;
	new_token = malloc(sizeof(t_token));
	new_token->next = NULL;
	new_token->type = token_type;
	new_token->value = NULL;
	if (!*token_last)
		*token_last = new_token;
	else
	{
		(*token_last)->next = new_token;
		*token_last = new_token;
	}
}

t_token_type	get_token(char **string)
{
	t_token_type	token;

	if (ft_strncmp(*string, "|", 1) == 0)
		token = T_PIPE;
	else if (ft_strncmp(*string, "(", 1) == 0)
		token = T_PARENTHESIS_OPEN;
	else if (ft_strncmp(*string, ")", 1) == 0)
		token = T_PARENTHESIS_CLOSE;
	else if (ft_strncmp(*string, "<", 1) == 0)
		token = T_REDIR_IN;
	else if (ft_strncmp(*string, ">", 1) == 0)
		token = T_REDIR_OUT;
	else if (!string[1])
		token = T_COMMAND;
	else if (ft_strncmp(*string, "&&", 2) == 0)
		token = T_AND;
	else if (ft_strncmp(*string, "||", 2) == 0)
		token = T_OR;
	else if (ft_strncmp(*string, ">>", 2) == 0)
		token = T_REDIR_APPEND;
	else if (ft_strncmp(*string, "<<", 2) == 0)
		token = T_REDIR_HEREDOC;
	else
		token = T_COMMAND;
	return (token);
}

void	tokenize_command(char *command)
{
	t_token			*token_first;
	t_token			*token_last;
	t_token_type	token_type;
	int				i;

	token_last = NULL;
	add_token(&token_last);
	token_first = token_last;
	token_type = get_token(&command);
	while (*command)
	{
		i = 0;
		while (token_type == T_COMMAND && command[i])
		{
			token_type = get_token(&command + i);
			i++;
		}
		if (i)
		{
			token_last->value = ft_substr(command, 0, i);
			command += i;
		}
		if (token_type != T_COMMAND)
		{
			add_token(&token_last, token_type);
			command += 1;
		}
		if (token_type == T_AND || token_type == T_OR
			|| token_type == T_REDIR_APPEND || token_type == T_REDIR_HEREDOC)
			command += 1;
	}
}
