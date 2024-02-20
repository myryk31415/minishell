/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/02/20 15:58:58 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	main(void)
{
	t_redirect_in	*in;
	t_redirect_out	*out;
	int				fd[2];
	int				pre_fd[2];
	int				id;
	in = malloc(sizeof(t_redirect_in *));
	in->heredoc = malloc(sizeof(bool *) * 4);
	in->heredoc[0] = false;
	in->heredoc[1] = false;
	in->heredoc[2] = false;
	in->heredoc[3] = NULL;
	in->string = malloc(sizeof(char *) * 4);
	in->string[0] = "test1.txt";
	in->string[1] = "README.md";
	in->string[2] = "test2.txt";
	in->string[3] = NULL;
	out = malloc(sizeof(t_redirect_out *));
	out->append = malloc(sizeof(bool *) * 4);
	out->append[0] = false;
	out->append[1] = false;
	out->append[2] = false;
	out->append[3] = NULL;
	out->cmd = "cat";
	out->string = malloc(sizeof(char *) * 4);
	out->string[0] = "test3.txt";
	out->string[1] = "hello.txt";
	out->string[2] = "test4.txt";
	out->string[3] = NULL;
	if (pipe(pre_fd) == -1)
		perror("Pipe");
	if (pipe(fd) == -1)
		perror("Pipe");
	input(in, pre_fd);
	id = fork();
	if (id == -1)
		perror("Fork");
	if (id == 0)
		output(out, fd);
	else
	{
		ft_printf("daisy");
		cmd("cat", fd, pre_fd);
	}
}
