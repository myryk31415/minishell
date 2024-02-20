/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:50 by padam             #+#    #+#             */
/*   Updated: 2024/02/20 18:31:00 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*add_token(t_token *token_last, t_token_type token_type)
{
	t_token	*new_token;

	if (token_last && token_last->type == T_WORD && token_type == T_WORD)
		return (token_last);
	new_token = malloc(sizeof(t_token));
	new_token->next = NULL;
	new_token->type = token_type;
	new_token->value = NULL;
	if (token_last)
		token_last->next = new_token;
	return (new_token);
}

//change to char pointer
t_token_type	get_token_type(char *string)
{
	t_token_type	token;

	if (is_separator(*string))
		token = T_SEPARATOR;
	else if (!ft_strncmp(string, "&&", 2))
		token = T_AND;
	else if (!ft_strncmp(string, "||", 2))
		token = T_OR;
	else if (!ft_strncmp(string, ">>", 2))
		token = T_REDIR_APPEND;
	else if (!ft_strncmp(string, "<<", 2))
		token = T_REDIR_HEREDOC;
	else if (!ft_strncmp(string, "(", 1))
		token = T_PARENTHESIS_OPEN;
	else if (!ft_strncmp(string, ")", 1))
		token = T_PARENTHESIS_CLOSE;
	else if (!ft_strncmp(string, "|", 1))
		token = T_PIPE;
	else if (!ft_strncmp(string, ">", 1))
		token = T_REDIR_OUT;
	else if (!ft_strncmp(string, "<", 1))
		token = T_REDIR_IN;
	else
		token = T_WORD;
	return (token);
}

//error handling
t_token	*handle_quotes(char **string, t_token *token_last)
{
	char	quote;
	int		i;

	i = 0;
	quote = **string;
	(*string)++;
	if (!**string)
		return (NULL);
	while ((*string)[i + 1] && (*string)[i] != quote)
		i++;
	if ((*string)[i] != quote)
		return (NULL);
	if (token_last && token_last->type == T_WORD)
		token_last->value = ft_strjoin(token_last->value,
				ft_substr(*string, 0, i));
	else
	{
		token_last = add_token(token_last, T_WORD);
		token_last->value = ft_substr(*string, 0, i);
	}
	*string += i;
	return (token_last);
}

t_token	*handle_command(char **string, t_token *token_last)
{
	t_token_type	token_type;
	int				i;

	i = 1;
	token_type = T_WORD;
	while (token_type == T_WORD && (*string)[i] && !is_quote((*string)[i]))
	{
		token_type = get_token_type((*string) + i);
		i++;
	}
	//careful
	if ((*string)[i] && !is_quote((*string)[i]))
		i--;
	// if (token_type == T_SEPARATOR)
	// 	i--;
	if (token_last && token_last->type == T_WORD)
		token_last->value = ft_strjoin(token_last->value,
				ft_substr(*string, 0, i));
	else
	{
		token_last = add_token(token_last, T_WORD);
		token_last->value = ft_substr(*string, 0, i);
	}
	*string += i - 1;
	return (token_last);
}

t_token	*get_next_token(char *string, t_token *token_last)
{
	t_token_type	token_type;
	int				i;

	token_type = get_token_type(string);
	token_last = add_token(token_last, token_type);
	i = 1;
	if (is_quote(*string))
		token_last = handle_quotes(&string, token_last);
	else if (token_type == T_WORD)
		token_last = handle_command(&string, token_last);
	else if (token_type == T_AND || token_type == T_OR
		|| token_type == T_REDIR_APPEND || token_type == T_REDIR_HEREDOC)
		i += 1;
	// else if (token_type == T_SEPARATOR)
	// {
	// 	while (is_separator(string[i - 1]))
	// 		i++;
	// }
	if (string[i])
		get_next_token(string + i, token_last);
	return (token_last);
}

void	debug_print_token_array(t_token *token_first)
{
	while (token_first)
	{
		printf("type: %d, value: %s\n", token_first->type, token_first->value);
		token_first = token_first->next;
	}
}

void	tokenize_command(char *command)
{
	t_token			*tokens;

	tokens = get_next_token(command, NULL);
	debug_print_token_array(tokens);
}

	// while (*command)
	// {
	// 	i = 0;
	// 	while (command[i])
	// 	{
	// 		token_type = get_token(&command + i);
	// 		if (token_type != T_COMMAND)
	// 			break ;
	// 		i++;
	// 	}
	// 	if (i)
	// 	{
	// 		add_token(&token_last, T_COMMAND);
	// 		token_last->value = ft_substr(command, 0, i);
	// 		command += i;
	// 	}
	// 	if (token_type != T_COMMAND)
	// 	{
	// 		add_token(&token_last, token_type);
	// 		command += 1;
	// 	}
	// 	if (token_type == T_AND || token_type == T_OR
	// 		|| token_type == T_REDIR_APPEND || token_type == T_REDIR_HEREDOC)
	// 		command += 1;
	// 	if (*command)
	// 		token_type = get_token(&command);
	// }
