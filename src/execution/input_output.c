/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/30 16:00:13 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	input_permission(char *input)
{
	int	file;

	file = 0;
	file = open(input, O_RDONLY, 0666);
	if (file == -1)
		return (error_msg("minishell: ", input), -1);
	return (file);
}

int	output_permission(char *output, int append)
{
	int	file;

	file = 0;
	if (append == 0)
		file = open(output,
				O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else
		file = open(output,
				O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (!file || file == -1)
		return (error_msg("minishell: ", output), -1);
	return (file);
}

int	handle_both_permission(int *redir, t_cmd *tk, t_exec *exec, int i)
{
	while (tk->redirects[i] || tk->redirect_type[i])
	{
		if (tk->redirect_type[i] == 0 || tk->redirect_type[i] == 1)
		{
			if (redir[1])
				close(redir[1]);
			redir[1] = output_permission(tk->redirects[i], \
				tk->redirect_type[i]);
		}
		else
		{
			if (redir[0])
				close(redir[0]);
			if (tk->redirect_type[i] == 2)
				redir[0] = input_permission(tk->redirects[i]);
			else if (tk->redirect_type[i] > 0)
				redir[0] = tk->redirect_type[i];
			else
				redir[0] = heredoc_expand(-tk->redirect_type[i], exec);
		}
		if (redir[0] == -1 || redir[1] == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	handle_both(int *redir, t_cmd *tk, t_exec *exec)
{
	int		i;

	i = 0;
	redir[0] = 0;
	redir[1] = 0;
	while (tk->redirects[i] || tk->redirect_type[i])
	{
		tk->redirects[i] = expand_variables(tk->redirects[i], exec);
		tk->redirects[i] = expander(tk->redirects[i]);
		i++;
	}
	i = 0;
	if (handle_both_permission(redir, tk, exec, i) == -1)
		return (-1);
	return (0);
}

void	redirect_nodes(t_redir *token, int **pipes, t_exec *exec)
{
	command(token->redirects, pipes, 1, exec);
	if (token->type == CMD)
		command((t_cmd *)token->next, pipes, 0, exec);
	else if (token->type == OR)
		or_execute((t_node *)token->next, pipes, exec);
	else if (token->type == AND)
		and_execute((t_node *)token->next, pipes, exec);
	else if (token->type == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->next, exec, pipes);
	else if (token->type == REDIR)
		redirect((t_redir *)token->next, pipes, 1, exec);
	free(pipes);
}

int	redirect(t_redir *token, int **pipes, int status, t_exec *exec)
{
	int	pid;
	int	pipe_exit;

	pipe_exit = 0;
	if (status == 1)
		pipe_exit = 1;
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid)
	{
		redirect_nodes(token, pipes, exec);
		exit_shell(exec, NULL, exec->exit_status);
	}
	else
	{
		close_pipes(pipes);
		exec->exit_status = new_waitpid(pid);
		if (pipe_exit)
		{
			free(pipes);
			exit_shell(exec, NULL, exec->exit_status);
		}
	}
	return (exec->exit_status);
}

void	error_msg(char *cmd, char *file)
{
	char	*tmp;

	tmp = ft_strjoin(cmd, file);
	perror(tmp);
	free(tmp);
}
