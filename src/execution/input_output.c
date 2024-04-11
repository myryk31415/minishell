/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/11 15:06:21 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	input_permission(char **input, int j)
{
	int	file;

	file = 0;
	if (access(input[j], R_OK))
	{
		file = open(input[j], O_RDONLY, 0666);
		if (file == -1)
			return (error_msg("minishell: ", input[j]), -1);
	}
	return (file);
}

int	output_permission(char **output, int *append, int j)
{
	int	file;

	file = 0;
	// if (access(output[j], W_OK))
	// {
		if (append[j] == 0)
			file = open(output[j],
					O_WRONLY | O_TRUNC | O_CREAT, 0666);
		else
			file = open(output[j],
					O_WRONLY | O_APPEND | O_CREAT, 0666);
	// }
	// else if (errno == EACCES)
	// {
	// 	ft_putstr_fd("minishell: Permission denied\n", 2);
	// 	exit(EXIT_FAILURE);
	// }
	// else if (errno == ENOENT)
	// {
	// 	ft_putstr_fd("minishell: No such file or directory\n", 2);
	// 	exit(EXIT_FAILURE);
	// }r
	if (!file || file == -1)
		return (error_msg("minishell: ", output[j]), -1);
	return (file);
}

int	input_handling(char **input, int *heredoc)
{
	int			j;
	int			file;

	file = 0;
	j = 0;
	while (input[j] || heredoc[j])
	{
		if (file)
			close(file);
		if (heredoc[j] == 0)
			file = input_permission(input, j);
		else
			file = heredoc[j];
		j++;
		if (file == -1)
			return (file);
	}
	return (file);
}

int	output_handling(char **output, int *append)
{
	int		file;
	int		j;

	j = 0;
	file = 0;
	while (output[j])
	{
		if (file)
			close(file);
		file = output_permission(output, append, j);
		j++;
		if (file == -1)
			return (file);
	}
	return (file);
}

void	redirect_nodes(t_redir *token, int **pipes, t_exec exec)
{
	command(token->redirects, pipes, 1, &exec);
	if (token->type == CMD)
		command((t_cmd *)token->next, pipes, 0, &exec);
	else if (token->type == OR)
		or_execute((t_node *)token->next, 1, pipes, &exec);
	else if (token->type == AND)
		and_execute((t_node *)token->next, 1, pipes, &exec);
	else if (token->type == PIPE)
		create_tree(0, (t_node *)token->next, exec);
	else if (token->type == REDIR)
		redirect((t_redir *)token->next, pipes, 1, exec);
}

int	redirect(t_redir *token, int **pipes, int status, t_exec exec)
{
	int	pid;
	int	pipe_exit;

	pipe_exit = 0;
	if (status == 1)
		pipe_exit = 1;
	pid = fork(	);
	if (pid == -1)
		perror("Fork");
	if (!pid)
	{
		redirect_nodes(token, pipes, exec);
		exit(EXIT_FAILURE);
	}
	else
	{
		close_pipes(pipes);
		waitpid(pid, &status, 0);
		if (pipe_exit)
			exit_shell(&exec, NULL, (unsigned int) status);
	}
	return (status);
}

void	error_msg(char *cmd, char *file)
{
	char	*tmp;

	tmp = ft_strjoin(cmd, file);
	perror(tmp);
	free(tmp);
}
