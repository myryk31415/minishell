/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:09:25 by padam             #+#    #+#             */
/*   Updated: 2024/02/15 15:09:19 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <stdbool.h>

typedef struct s_redirect_in
{
	char	*string;
	bool	heredoc;
}	t_redirect;

typedef struct s_redirect_out
{
	char	*string;
	bool	append;
}	t_redirect_out;

typedef enum e_node_type
{
	AND,
	OR,
	PIPE,
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
