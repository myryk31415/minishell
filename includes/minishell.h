/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:09:25 by padam             #+#    #+#             */
/*   Updated: 2024/02/14 19:37:13 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"

typedef enum e_node_type
{
	AND,
	OR,
	PIPE,
	REDIR_IN,
	REDIR_DELIM,
	REDIR_OUT,
	REDIR_APPEND,
	CMD
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	char			*command;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

#endif
