/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:09:25 by padam             #+#    #+#             */
/*   Updated: 2024/03/08 12:06:12 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include "parser.h"

typedef struct s_cmd
{
	char	**args;
	char	**redirect_in;
	bool	*heredoc;
	char	**redirect_out;
	bool	*append;
}	t_cmd;

typedef enum e_node_type
{
	ERROR,
	AND,
	OR,
	PIPE,
	CMD,
}	t_node_type;

typedef struct s_node
{
	void		*left;
	t_node_type	type_left;
	bool		new_process_left;
	void		*right;
	t_node_type	type_right;
	bool		new_process_right;
}	t_node;

void	parser(void);

#endif
