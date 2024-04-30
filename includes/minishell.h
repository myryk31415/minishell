/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:09:25 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 21:18:41 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include <dirent.h>
# include <termios.h>
# define DEBUG 1

typedef struct s_cmd
{
	char	**args;
	char	**redirects;
	int		*redirect_type;
}	t_cmd;

typedef enum e_node_type
{
	ERROR,
	REDIR,
	AND,
	OR,
	PIPE,
	CMD,
}	t_node_type;

typedef struct s_redir
{
	void		*next;
	t_node_type	type;
	t_cmd		*redirects;
}	t_redir;

typedef struct s_node
{
	void		*left;
	t_node_type	type_left;
	void		*right;
	t_node_type	type_right;
}	t_node;

typedef struct s_exec
{
	char		***env;
	void		*tree;
	t_node_type	type;
	int			sub_process;
	int			exit_status;
}	t_exec;

// commands
t_node_type	parser(void **token_tree, t_exec *exec);
char		*get_env(char **environ, char *var);
void		execution(void *tree, t_node_type type, t_exec *exec);
int			exit_shell(t_exec *exec, char **arg, int status);

// variable_expansion
char		*expand_tilde(char *arg, char **env);
char		*expand_variables(char *command, t_exec *exec);
char		*expander(char *arg);
char		**expander_array(char **args, t_exec *exec);
int			heredoc_expand(int heredoc, t_exec *exec);
char		**word_splitting(char **args);

//signal
void		parser_handler(int signal);
void		execution_handler(int signal);
void		ft_restore_terminal(int i);
void		ft_configure_terminal(void);

//cleanup
void		free_str_array(char **arr);
void		cmd_free(t_cmd *cmd);
void		node_tree_delete(void *node, t_node_type type);
void		free_env(char ***env);

#endif
