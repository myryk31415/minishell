/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:09:25 by padam             #+#    #+#             */
/*   Updated: 2024/02/20 15:53:58 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_redirect_in
{
	char	**string;
	bool	*heredoc;
}	t_redirect_in;

typedef struct s_redirect_out
{
	char	**string;
	bool	*append;
}	t_redirect_out;

typedef struct s_cmd
{
	char			**args;
	t_redirect_in	redirect_in;
	t_redirect_out	redirect_out;
}	t_cmd;

typedef enum e_node_type
{
	GROUP,
	AND,
	OR,
	PIPE,
	CMD,
}	t_node_type;

typedef struct s_node
{
	void		*left;
	t_node_type	type_left;
	void		*right;
	t_node_type	type_right;
}	t_node;

typedef struct s_node_group
{
	t_node		*node;
	t_node_type	*type;
}	t_group;

void	parser(void);

#endif
