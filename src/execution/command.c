/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:38:37 by antonweizma       #+#    #+#             */
/*   Updated: 2024/03/28 14:13:37 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	in_and_out_handling(t_cmd *token, int **pipes, int *redir)
{
	int	input;
	int	output;

	input = 0;
	output = 1;
	if ((token->redirect_in && *(token->redirect_in)) || \
	(token->heredoc && *(token->heredoc)))
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
	close_in_and_out_files(input, output, redir, pipes);
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
	if (exec->exit_status == 1)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			in_and_out_handling(token, pipes, redir);
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
	if (exec->exit_status == 1)
	{
		in_and_out_handling(token, pipes, redir);
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
			redir[0] = input_handling(token->redirect_in, token->heredoc);
		if (token->redirect_out && *(token->redirect_out))
			redir[1] = output_handling(token->redirect_out, token->append);
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
