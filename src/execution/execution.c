/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/25 11:47:17 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	command(t_cmd *token, int **pipes, int *redir)
{
	int	input;
	int	output;

	input = 0;
	output = 1;
	if (token->redirect_in && *(token->redirect_in))
		input = input_handling(token->redirect_in, token->heredoc);
	else if (redir && redir[0])
	{
		input = redir[0];
		redir[0] = 0;
	}
	else if (pipes && pipes[1])
		input = pipes[1][0];
	dup2(input, STDIN_FILENO);
	if (token->redirect_out && *(token->redirect_out))
		output = output_handling(token->redirect_out, token->append);
	else if (redir && redir[1])
		output = redir[1];
	else if (pipes && pipes[0])
		output = pipes[0][1];
	dup2(output, STDOUT_FILENO);
	// if (output != 1 && redir && !redir[1])
	// 	close(output);
	close_pipes(pipes);
}

int	command_no_pipe(t_cmd *token, char ***env)
{
	int			status;
	int			id;
	int			old_stdin;
	int			old_stdout;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	status = is_builtin(token, NULL, NULL, env);
	if (status == 1)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			command(token, NULL, NULL);
			exec(token->args, *env);
		}
		else
			waitpid(id, &status, 0);
	}
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
	return (status);
}

void	command_pipe(t_cmd *token, int **pipes, int redirect, char ***env)
{
	static int	redir[2];
	int			status;

	if (redirect == 1)
	{
		if (token->redirect_in && *(token->redirect_in))
			redir[0] = input_handling(token->redirect_in, token->heredoc);
		if (token->redirect_out && *(token->redirect_out))
			redir[1] = output_handling(token->redirect_out, token->append);
		return ;
	}
	else
	{
		status = is_builtin(token, pipes, redir, env);
		if (status == 1)
		{
			command(token, pipes, redir);
			exec(token->args, *env);
		}
		exit(0);
	}
}

int	create_tree(int *pre_fd, t_node *token, int status, char **env)
{
	int	fd[2];
	int	pid;
	int	**pipes;

	pipes = malloc(sizeof(int *) * 2 + 1);
	if (!pipes)
		perror("Malloc");
	pipes[0] = fd;
	pipes[1] = pre_fd;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pipe(pipes[0]) == -1)
		perror("Pipe");
	else if (!pid)
		run_tree(token, pipes, &env);
	else
	{
		waitpid(pid, &status, 0);
		close_pipes(pipes);
	}
	return (status);
}

void	run_tree(t_node *token, int **pipes, char ***env)
{
	int		pid;

	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid && token->type_left == CMD)
		command_pipe((t_cmd *)token->left, pipes, 0, env);
	else if (!pid && token->type_left == REDIR)
		redirect((t_redir *)token->left, pipes, 0, *env);
	if (pid && token->type_right == PIPE)
		create_tree(pipes[0], (t_node *)token->right, 0, *env);
	else if (pid && token->type_right == CMD)
	{
		pipes[1] = pipes[0];
		pipes[0] = NULL;
		command_pipe((t_cmd *)token->right, pipes, 0, env);
	}
	else if (pid && token->type_right == REDIR)
	{
		pipes[1] = pipes[0];
		pipes[0] = NULL;
		redirect((t_redir *)token->right, pipes, 0, *env);
	}
}

void	execution(void *tree, t_node_type type, char ***env)
{
	if (type == CMD)
		command_no_pipe((t_cmd *)tree, env);
	else if (type == AND)
		and_execute((t_node *)tree, 0, env);
	else if (type == OR)
		or_execute((t_node *)tree, 0, env);
	else if (type == PIPE)
		create_tree(0, (t_node *)tree, 0, *env);
	else if (type == REDIR)
		redirect((t_redir *)tree, NULL, 0, *env);
}
