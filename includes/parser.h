/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:14:04 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 15:41:16 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// colors
# define RED_INCIDATOR "\002\001\e[1m\e[31m●\e[0m\002"
# define MAGENTA_INCIDATOR "\002\001\e[1m\e[35m●\e[0m\002"
# define RESET "\e[0m\002"
# define RED "\002\001\e[1m\e[31m"
# define GREEN "\002\001\e[1m\e[32m"
# define YELLOW "\002\001\e[1m\e[33m"
# define BLUE "\002\001\e[1m\e[34m"
# define MAGENTA "\002\001\e[1m\e[35m"
# define CYAN "\002\001\e[1m\e[36m"
//# background="\033[48;5;93m"

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				quote;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

char	*check_unclosed(char *command, t_exec *exec);

///BIN_TREE
// node_struct
t_node_type	tokens_to_tree(t_token *tokens, void **head);

// node_utils
t_node		*new_node(void);
t_redir		*new_redir_node(void);

// pipeline
int			count_words(t_token *tokens);
t_token		*get_pipe(t_token *tokens);
t_token		*get_operator(t_token *tokens);

// redirects
int			redirects_get(t_token **token_first, t_cmd **redirects);
int			climb_tree(void *ptr, t_node_type type);

///REST
//cleanup
t_node_type	err_pars(char *message, t_cmd *redirects, t_token **tokens);
void		print_syntax_err(t_token *token);

// debug
void		debug_print_token_array(t_token *token_first);
void		get_next_debug(void *ptr, t_node_type type, int i);

// is_checks
int			is_quote(char c);
int			is_separator(char c);
int			is_redirect(t_token_type type);
int			is_operator(t_token_type type);
int			is_variable(char c);

// tokenizer
t_token_type	get_token_type(char *string);
t_token		*get_next_token(char *string, t_token *token_last);

// tokens utils
void		token_delete(t_token **tokens);
void		token_delete_all(t_token **tokens);
t_token		*token_add(t_token *tokens, t_token_type token_type);
t_token		*token_split(t_token *tokens, int direction);
t_token		*skip_parens(t_token *tokens, int direction);

#endif
