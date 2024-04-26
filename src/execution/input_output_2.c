/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:38:37 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/26 21:43:10 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	redir_input(int **pipes, int *redir, int *input_output)
{
	int	input;

	input = 0;
	if (input_output[0])
		input = input_output[0];
	else if (redir && redir[0])
	{
		input = redir[0];
		redir[0] = 0;
	}
	else if (pipes && pipes[1])
		input = pipes[1][0];
	return (input);
}

int	redir_output(int **pipes, int *redir, int *input_output)
{
	int	output;

	output = 1;
	if (input_output[1])
		output = input_output[1];
	else if (redir && redir[1])
		output = redir[1];
	else if (pipes && pipes[0])
		output = pipes[0][1];
	return (output);
}

int	in_and_out_hdl_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int	input_output[2];
	int	input;
	int	output;

	handle_both(input_output, token, exec);
	input = redir_input(pipes, redir, input_output);
	if (input == -1)
		return (EXIT_FAILURE);
	dup2(input, STDIN_FILENO);
	output = redir_output(pipes, redir, input_output);
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

	handle_both(input_output, token, exec);
	input = redir_input(pipes, redir, input_output);
	if (input == -1)
		return (EXIT_FAILURE);
	dup2(input, STDIN_FILENO);
	output = redir_output(pipes, redir, input_output);
	if (output == -1)
		return (EXIT_FAILURE);
	dup2(output, STDOUT_FILENO);
	close_in_out_files_fork(pipes);
	return (EXIT_SUCCESS);
}
