/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/01 11:02:14 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

// int	main(void)
// {
// 	t_redirect_in	*in;
// 	t_redirect_out	*out;
// 	t_cmd			*cmd;
// 	int				fd[2];
// 	int				pre_fd[2];

// 	in = ft_calloc(sizeof(t_redirect_in), 1);
// 	in->heredoc = ft_calloc(sizeof(bool), 4);
// 	in->heredoc[0] = false;
// 	in->heredoc[1] = false;
// 	in->heredoc[2] = false;
// 	in->heredoc[3] = NULL;
// 	in->string = ft_calloc(sizeof(char *), 4);
// 	in->string[0] = "test1.txt";
// 	in->string[1] = "README.md";
// 	in->string[2] = "test2.txt";
// 	in->string[3] = NULL;
// 	out = ft_calloc(sizeof(t_redirect_out), 1);
// 	out->append = ft_calloc(sizeof(bool), 4);
// 	out->append[0] = false;
// 	out->append[1] = false;
// 	out->append[2] = false;
// 	out->append[3] = NULL;
// 	out->string = ft_calloc(sizeof(char *), 4);
// 	out->string[0] = "test3.txt";
// 	out->string[1] = "hello.txt";
// 	out->string[2] = "test4.txt";
// 	out->string[3] = NULL;
// 	cmd = malloc(sizeof(t_cmd));
// 	cmd->redirect_in = in;
// 	cmd->redirect_out = out;
// 	cmd->args = malloc(sizeof(char *) * 2);
// 	cmd->args[0] = "cat";
// 	cmd->args[1] = NULL;
// 	if (pipe(pre_fd) == -1)
// 		perror("Pipe");
// 	if (pipe(fd) == -1)
// 		perror("Pipe");
// 	command(cmd, fd, pre_fd);
// }
