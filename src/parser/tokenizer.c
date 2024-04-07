/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:40:50 by padam             #+#    #+#             */
/*   Updated: 2024/04/07 21:57:30 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
		token = T_LPAREN;
	else if (!ft_strncmp(string, ")", 1))
		token = T_RPAREN;
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

char	*var_expand(char *old_string, char *string, int quotes, t_exec *exec)
{
	char *output;

	string = expand_variables(string, exec);
	if (!string && !old_string && !quotes)
		return (NULL);
	output = ft_strjoin(old_string, string);
	free(old_string);
	free(string);
	return (output);
}

//error handling
t_token	*handle_quotes(char **string, t_token *token_last, t_exec *exec)
{
	char	quote;
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	quote = **string;
	(*string)++;
	if (!**string)
		return (NULL);
	while ((*string)[i + 1] && (*string)[i] != quote)
		i++;
	if ((*string)[i] != quote)
		return (NULL);
	if (!token_last || token_last->type != T_WORD)
		token_last = token_add(token_last, T_WORD);
	if (quote == '\'')
	{
		tmp2 = ft_substr(*string, 0, i);
		tmp = token_last->value;
		token_last->value = ft_strjoin(tmp, tmp2);
		free(tmp);
		free(tmp2);
	}
	else
		token_last->value = var_expand(token_last->value, ft_substr(*string, 0, i), 1, exec);
	token_last->quote = 2;
	*string += i;
	return (token_last);
}

t_token	*handle_word(char **string, t_token *token_last, t_exec *exec)
{
	t_token_type	token_type;
	int				i;

	i = 1;
	token_type = T_WORD;
	while (token_type == T_WORD && (*string)[i] && !is_quote((*string)[i]))
		token_type = get_token_type((*string) + i++);
	if (token_type != T_WORD)
		i--;
	if (!token_last || token_last->type != T_WORD)
		token_last = token_add(token_last, T_WORD);
	token_last->value = var_expand(token_last->value,
		ft_substr(*string, 0, i), 0, exec);
	*string += i - 1;
	return (token_last);
}

t_token	*get_next_token(char *string, t_token *token_last, t_exec *exec)
{
	t_token_type	token_type;
	int				i;

	i = 1;
	if (is_separator(*string) && !(*(string + 1)))
		return (token_last);
	token_type = get_token_type(string);
	if (is_separator(*string) && is_separator(*(string + 1)))
		return (get_next_token(string + 1, token_last, exec));
	token_last = token_add(token_last, token_type);
	if (is_quote(*string))
		token_last = handle_quotes(&string, token_last, exec);
	else if (token_type == T_WORD)
		token_last = handle_word(&string, token_last, exec);
	else if (token_type == T_AND || token_type == T_OR
		|| token_type == T_REDIR_APPEND || token_type == T_REDIR_HEREDOC)
		i += 1;
	if (string[i])
		token_last = get_next_token(string + i, token_last, exec);
	return (token_last);
}

// t_token	*tokenize_command(char *command, char **env)
// {
// 	//just call get_next_token directly
// 	t_token			*tokens;

// 	tokens = get_next_token(command, NULL, env);
// 	return (tokens);
// }
