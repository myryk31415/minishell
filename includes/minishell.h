/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:09:25 by padam             #+#    #+#             */
/*   Updated: 2024/03/12 08:40:49 by antonweizma      ###   ########.fr       */
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

typedef struct s_cmd
{
	char	**args;
	int		redirect_in;
	int		redirect_out;
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
	bool		new_process_left;
	void		*right;
	t_node_type	type_right;
	bool		new_process_right;
}	t_node;

t_node_type	parser(void **tree);

#endif
