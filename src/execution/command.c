/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:38:37 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/26 13:22:38 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// int	in_handling(t_cmd *token, int **pipes, int *redir, t_exec *exec)
// {
// 	int	input;


// 	input = 0;
// 	if ((token->redirect_in && *(token->redirect_in)) || (token->heredoc && *(token->heredoc)))
// 		input = input_handling(token->redirect_in, token->heredoc, exec);
// 	else if (redir && redir[0])
// 	{
// 		input = redir[0];
// 		redir[0] = 0;
// 	}
// 	else if (pipes && pipes[1])
// 		input = pipes[1][0];
// 	if (input == -1)
// 		return (input);
// 	dup2(input, STDIN_FILENO);
// 	return (0);
// }

// int	in_and_out_handling(t_cmd *token, int **pipes, int *redir, t_exec *exec)
// {
// 	int	output;
// 	int	input;

// 	output = 1;
// 	input = in_handling(token, pipes, redir, exec);
// 	if (input == -1)
// 		return (EXIT_FAILURE);
// 	if (token->redirect_out && *(token->redirect_out))
// 		output = output_handling(token->redirect_out, token->append, exec);
// 	else if (redir && redir[1])
// 		output = redir[1];
// 	else if (pipes && pipes[0])
// 		output = pipes[0][1];
// 	if (output == -1)
// 		return (EXIT_FAILURE);
// 	dup2(output, STDOUT_FILENO);
// 	close_in_and_out_files(input, output, redir, pipes);
// 	return (EXIT_SUCCESS);
// }

int	in_and_out_hdl_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int	input_output[2];
	int	input;
	int	output;

	input = 0;
	output = 1;
	handle_both(input_output, token, exec);
	if (input_output[0])
		input = input_output[0];
	else if (redir && redir[0])
	{
		input = redir[0];
		redir[0] = 0;
	}
	else if (pipes && pipes[1])
		input = pipes[1][0];
	if (input == -1)
		return (EXIT_FAILURE);
	dup2(input, STDIN_FILENO);
	if (input_output[1])
		output = input_output[1];
	else if (redir && redir[1])
		output = redir[1];
	else if (pipes && pipes[0])
		output = pipes[0][1];
	if (output == -1)
		return (EXIT_FAILURE);
	dup2(output, STDOUT_FILENO);
	close_in_out_file_nofork(input, output, redir, pipes);
	return (EXIT_SUCCESS);
}

int	in_and_out_hdl_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int	input_output[2];
	int	input;
	int	output;

	input = 0;
	output = 1;
	handle_both(input_output, token, exec);
	if (input_output[0])
		input = input_output[0];
	else if (redir && redir[0])
	{
		input = redir[0];
		redir[0] = 0;
	}
	else if (pipes && pipes[1])
		input = pipes[1][0];
	if (input == -1)
		return (EXIT_FAILURE);
	dup2(input, STDIN_FILENO);
	if (input_output[1])
		output = input_output[1];
	else if (redir && redir[1])
		output = redir[1];
	else if (pipes && pipes[0])
		output = pipes[0][1];
	if (output == -1)
		return (EXIT_FAILURE);
	dup2(output, STDOUT_FILENO);
	close_in_out_files_fork(input, output, redir, pipes);
	return (EXIT_SUCCESS);
}

int	builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int		old_stdin;
	int		old_stdout;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	exec->exit_status = is_builtin(token, pipes, redir, exec);
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
	if (exec->exit_status == -1)
	{
		exec->exit_status = 0;
		return (-1);
	}
	return (exec->exit_status);
}

void	command_fork(t_cmd *token, t_exec *exec, int **pipes, int *redir)
{
	int			id;
	char		**tmp;
	int			status;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	if (builtin(token, pipes, redir, exec) == -1)
	{
		id = fork();
		if (id == -1)
			perror("Fork");
		if (!id)
		{
			if (in_and_out_hdl_fork(token, pipes, redir, exec) == 1)
				exit_shell(exec, NULL, EXIT_FAILURE, 1);
			tmp = token->args;
			token->args = NULL;
			execute(tmp, exec);
		}
		else
		{
			close_pipes(pipes);
			waitpid(id, &status, 0);
			if (WIFEXITED(status))
				exec->exit_status = WEXITSTATUS(status);
		}
	}
}

void	command_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	char		**tmp;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	exec->exit_status = is_builtin_no_fork(token, pipes, redir, exec);
	if (exec->exit_status == -1)
	{
		exec->exit_status = 0;
		if (in_and_out_hdl_no_fork(token, pipes, redir, exec) == 1)
			exit_shell(exec, NULL, EXIT_FAILURE, 1);
		tmp = token->args;
		token->args = NULL;
		execute(tmp, exec);
	}
	close_pipes(pipes);
	// ft_putstr_fd(token->args[0], 2);
	// ft_putstr_fd(token->args[1], 2);
	// ft_putstr_fd("\n EXITSTATUS: ", 2);
	// ft_putnbr_fd(exec->exit_status, 2);
	// ft_putstr_fd("\n", 2);
	exit_shell(exec, NULL, exec->exit_status, 1);
}

void	command(t_cmd *token, int **pipes, int redirect, t_exec *exec)
{
	static int	redir[2];

	if (redirect == 1)
	{
		if (token->redirects && *(token->redirects))
			handle_both(redir, token, exec);
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
