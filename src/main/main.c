/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/21 22:48:17 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	main(void)
{
	void		*token_tree;
	t_node_type	type;
	int			exit_status;

	exit_status = 0;
	while (1)
	{
		token_tree = NULL;
		type = parser(token_tree, exit_status);
		execution(token_tree, type);
	}
	return (0);
}
