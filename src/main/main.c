/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/02/19 13:01:35 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	main(void)
{
	t_redirect_in	*in;
	int				fd[2];

	in = malloc(sizeof(t_redirect_in *));
	in->heredoc = malloc(sizeof(bool *));
	in->heredoc[0] = false;
	in->heredoc[1] = false;
	in->heredoc[2] = false;
	in->heredoc[3] = false;
	in->string = malloc(sizeof(char *));
	in->string[0] = "test1.txt";
	in->string[1] = "hello.txt";
	in->string[3] = "test2.txt";
	in->string[2] = "peter.txt";
	in->string[4] = NULL;

	if (pipe(fd) == -1)
		perror("Pipe");
	input(in, fd, 1);
	dup2(fd[0], STDIN_FILENO);
	exec("cat");
}
