/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/27 21:24:20 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	create_tree(int *pre_fd, t_node *token, int status, char **env)
{
	int	fd[2];
	int	**pipes;
	int	pid;
	int	id;

	id = 1;
	pipes = malloc(sizeof(int *) * 2);
	if (!pipes)
		perror("Malloc");
	if (pipe(fd) == -1)
		perror("Pipe");
	pipes[0] = fd;
	pipes[1] = pre_fd;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid && token->type_left == CMD)
		command((t_cmd *)token->left, pipes, 0, &env);
	else if (!pid && token->type_left == REDIR)
		redirect((t_redir *)token->left, pipes, 1, env);
	run_tree(token, pipes, &env, id);
	waitpid(pid, NULL, 0);
	free(pipes);
	return (status);
}

void	run_tree(t_node *token, int **pipes, char ***env, int id)
{
	if (token->type_right != PIPE)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			close_pipe(pipes[1]);
			pipes[1] = pipes[0];
			pipes[0] = NULL;
			if (token->type_right == CMD)
				command((t_cmd *)token->right, pipes, 0, env);
			else if (token->type_right == REDIR)
				redirect((t_redir *)token->right, pipes, 1, *env);
		}
	}
	if (id && token->type_right == PIPE)
	{
		close_pipe(pipes[1]);
		create_tree(pipes[0], (t_node *)token->right, 0, *env);
		close_pipes(pipes);
	}
	close_pipes(pipes);
	waitpid(id, NULL, 0);
}

void	execution(void *tree, t_node_type type, char ***env)
{

	if (type == CMD)
		command((t_cmd *)tree, NULL, 2, env);
	else if (type == AND)
		and_execute((t_node *)tree, 0, NULL, env);
	else if (type == OR)
		or_execute((t_node *)tree, 0, NULL, env);
	else if (type == PIPE)
		create_tree(0, (t_node *)tree, 1, *env);
	else if (type == REDIR)
		redirect((t_redir *)tree, NULL, 0, *env);
}
