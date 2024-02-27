/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:14:04 by padam             #+#    #+#             */
/*   Updated: 2024/02/27 17:08:57 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

// colors
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

typedef enum e_token_type
{
	T_WORD,
	T_SEPARATOR,
	T_PIPE,
	T_AND,
	T_OR,
	T_LPAREN,
	T_RPAREN,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC,
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
} t_token;

// node_struct
t_node *new_node(void);
int		count_operators(t_token *tokens);

// parser_utils
int		is_quote(char c);
int		is_separator(char c);
int		is_redirect(t_token_type type);
int		is_operator(t_token_type type);

// redirects
t_cmd	*redirects_dup(t_cmd *redirects);
t_cmd	*redirects_get(t_token *tokens, t_cmd *redirects);

// tokenizer
t_token	*tokenize_command(char *command);

// tokens utils
void	token_delete(t_token **tokens);
t_token	*token_add(t_token *tokens, t_token_type token_type);
t_token	*token_split(t_token *tokens);
t_token	*skip_parens(t_token *tokens);
t_token	*get_operator(t_token **tokens);

#endif
