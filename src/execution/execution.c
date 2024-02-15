/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/15 16:49:01 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	execution_start(t_node *token, t_redirect_in *input, t_redirect_out *output)
{

}

int	execution(t_node *token, t_redirect_in *input, t_redirect_out *output)
{
	if (token->type == PIPE)
		create_pipe(token->left, token->right);
	if (token->type == CMD)
		exec(token->command);
}

void	create_pipe(t_node left, t_node right)
{
	int	fd[2];
	int	id;

	+
}
