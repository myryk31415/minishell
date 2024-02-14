/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 13:15:13 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/14 16:47:50 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	initiate_child(t_args *args, int *pre_fd)
{
	int	file;

	file = 0;
	if (args->here_doc)
	{
		dup2(args->here_doc_pipe[0], STDIN_FILENO);
		close(args->here_doc_pipe[0]);
		close(args->here_doc_pipe[1]);
	}
	else
	{
		file = open(args->argv[1], O_RDONLY, 0666);
		if (file == -1)
			perror("Infile");
		dup2(file, STDIN_FILENO);
		close(file);
	}
	close(pre_fd[0]);
	dup2(pre_fd[1], STDOUT_FILENO);
	close(pre_fd[1]);
	exec(args->argv[2 + args->here_doc]);
}

void	child(t_args *args, int *fd, int *pre_fd, int cmd)
{
	dup2(fd[0], STDIN_FILENO);
	dup2(pre_fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(pre_fd[0]);
	close(pre_fd[1]);
	exec(args->argv[args->argc - 1 - cmd]);
}

void	parent(t_args *args, int *fd)
{
	int	file;

	if (args->here_doc)
		file = open(args->argv[args->argc - 1],
				O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		file = open(args->argv[args->argc - 1],
				O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (file == -1)
		perror("Outfile");
	dup2(fd[0], STDIN_FILENO);
	dup2(file, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file);
	exec(args->argv[args->argc - 2]);
}

void	fork_tree(int *pre_fd, t_args *args, int commands)
{
	int		fd[2];
	int		pid;

	if (commands < args->argc - 3 - args->here_doc)
	{
		if (pipe(fd) == -1)
			perror("Pipe");
		pid = fork();
		if (pid == -1)
			perror("Fork");
		if (pid == 0)
			fork_tree(fd, args, commands + 1);
		else if (pid != 0 && commands == 1)
			parent(args, fd);
		else
			child(args, fd, pre_fd, commands);
	}
	else if (commands == args->argc - 3 - args->here_doc)
	{
		initiate_child(args, pre_fd);
	}
}
