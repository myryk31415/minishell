/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:38:37 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/12 00:41:05 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	in_handling(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int	input;


	input = 0;
	if ((token->redirect_in && *(token->redirect_in)) || \
	(token->heredoc && *(token->heredoc)))
		input = input_handling(token->redirect_in, token->heredoc, exec);
	else if (redir && redir[0])
	{
		input = redir[0];
		redir[0] = 0;
	}
	else if (pipes && pipes[1])
		input = pipes[1][0];
	if (input == -1)
		return (input);
	dup2(input, STDIN_FILENO);
	return (0);
}

int	in_and_out_handling(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int	output;
	int	input;

	output = 1;
	input = in_handling(token, pipes, redir, exec);
	if (input == -1)
		return (EXIT_FAILURE);
	if (token->redirect_out && *(token->redirect_out))
		output = output_handling(token->redirect_out, token->append, exec);
	else if (redir && redir[1])
		output = redir[1];
	else if (pipes && pipes[0])
		output = pipes[0][1];
	if (output == -1)
		return (EXIT_FAILURE);
	dup2(output, STDOUT_FILENO);
	close_in_and_out_files(input, output, redir, pipes);
	return (EXIT_SUCCESS);
}

void	builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int		old_stdin;
	int		old_stdout;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	exec->exit_status = is_builtin(token, pipes, redir, exec);
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
}

void	command_fork(t_cmd *token, t_exec *exec, int **pipes, int *redir)
{
	int			id;
	char		**tmp;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	builtin(token, pipes, redir, exec);
	if (exec->exit_status == -1)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			if (in_and_out_handling(token, pipes, redir, exec) == 1)
				exit_shell(exec, NULL, EXIT_FAILURE);
			tmp = token->args;
			token->args = NULL;
			execute(tmp, exec);
		}
		else
		{
			close_pipes(pipes);
			waitpid(id, &exec->exit_status, 0);
		}
	}
}

void	command_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	char		**tmp;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	exec->exit_status = is_builtin(token, pipes, redir, exec);
	if (exec->exit_status == -1)
	{
		in_and_out_handling(token, pipes, redir, exec);
		tmp = token->args;
		token->args = NULL;
		execute(tmp, exec);
	}
	close_pipes(pipes);
	exit_shell(exec, NULL, exec->exit_status);
}

void	command(t_cmd *token, int **pipes, int redirect, t_exec *exec)
{
	static int	redir[2];

	if (redirect == 1)
	{
		if (token->redirect_in && *(token->redirect_in))
			redir[0] = input_handling(token->redirect_in, token->heredoc, exec);
		if (token->redirect_out && *(token->redirect_out))
			redir[1] = output_handling(token->redirect_out, token->append, exec);
		return ;
	}
	else
	{
		if (redirect == 0)
			command_no_fork(token, pipes, redir, exec);
		else
			command_fork(token, exec, pipes, redir);
	}
}
