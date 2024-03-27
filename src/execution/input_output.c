/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/27 17:36:15 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	input_handling(char **input, int *heredoc)
{
	int			j;
	int			file;

	file = 0;
	j = 0;
	while (input[j])
	{
		if (file)
			close(file);
		if (heredoc[j] == 0)
		{
			file = open(input[j], O_RDONLY, 0666);
			if (file == -1)
			{
				error_msg("minishell: ", input[j]);
				exit(EXIT_FAILURE);
			}
		}
		else
			file = heredoc[j];
		j++;
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
		if (append[j] == 0)
			file = open(output[j],
					O_WRONLY | O_TRUNC | O_CREAT, 0666);
		else
			file = open(output[j],
					O_WRONLY | O_APPEND | O_CREAT, 0666);
		if (file == -1)
		{
			error_msg("minishell: ", output[j]);
			exit(EXIT_FAILURE);
		}
		j++;
	}
	return (file);
}

void	redirect_nodes(t_redir *token, int **pipes, char **env)
{
	command(token->redirects, pipes, 1, &env);
	if (token->type == CMD)
		command((t_cmd *)token->next, pipes, 0, &env);
	else if (token->type == OR)
		or_execute((t_node *)token->next, 1, pipes, &env);
	else if (token->type == AND)
		and_execute((t_node *)token->next, 1, pipes, &env);
	else if (token->type == PIPE)
		create_tree(0, (t_node *)token->next, 1, env);
	else if (token->type == REDIR)
		redirect((t_redir *)token->next, pipes, 1, env);
}

int	redirect(t_redir *token, int **pipes, int status, char **env)
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
		redirect_nodes(token, pipes, env);
		exit(EXIT_FAILURE);
	}
	else
	{
		close_pipes(pipes);
		waitpid(pid, &status, 0);
		if (pipe_exit)
			exit(status);
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
