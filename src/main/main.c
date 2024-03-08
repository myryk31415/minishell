/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/08 11:58:37 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	main(void)
{
	t_cmd			*cmd_l;
	t_cmd			*cmd_r;
	t_node			*and;
	int				fd[2];
	int				pre_fd[2];

	and = malloc(sizeof(t_node));
	and->type_left =
	cmd_l = malloc(sizeof(t_cmd));
	cmd_l->args = malloc(sizeof(char *) * 2);
	cmd_l->args[0] = "echo a";
	cmd_l->args[1] = NULL;
	cmd_r = malloc(sizeof(t_cmd));
	cmd_r->args = malloc(sizeof(char *) * 2);
	cmd_r->args[0] = "cat";
	cmd_r->args[1] = NULL;
	if (pipe(pre_fd) == -1)
		perror("Pipe");
	if (pipe(fd) == -1)
		perror("Pipe");
	and_execute(cmd, NULL, NULL, 0);
}
