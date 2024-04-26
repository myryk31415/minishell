/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/26 20:20:28 by padam            ###   ########.fr       */
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
		return (error_msg("minishell: ", output), -1);
	return (file);
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
		// if (tmp && tmp[1])
		// {
		// 	//free ??
		// 	ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		// 	return (-1);
		// }
		i++;
	}
	i = 0;
	while (tk->redirects[i] || tk->redirect_type[i])
	{
		if (tk->redirect_type[i] == 0 || tk->redirect_type[i] == 1)
		{
			if (redir[1])
				close(redir[1]);
			redir[1] = output_permission(tk->redirects[i], tk->redirect_type[i]);
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

// int	input_handling(char **input, int *heredoc, t_exec *exec)
// {
// 	int			j;
// 	int			file;

// 	file = 0;
// 	j = 0;
// 	input = expander_array(input, exec);
// 	while (input[j] || heredoc[j])
// 	{
// 		if (file)
// 			close(file);
// 		if (heredoc[j] == 0)
// 			file = input_permission(input, j);
// 		else
// 			file = heredoc[j];
// 		j++;
// 		if (file == -1)
// 			return (file);
// 	}
// 	return (file);
// }

// int	output_handling(char **output, int *append, t_exec *exec)
// {
// 	int			j;
// 	int			file;

// 	file = 0;
// 	j = 0;
// 	output = expander_array(output, exec);
// 	while (output[j])
// 	{
// 		if (file)
// 			close(file);
// 		file = output_permission(output, append, j);
// 		j++;
// 		if (file == -1)
// 			return (file);
// 	}
// 	return (file);
// }

void	redirect_nodes(t_redir *token, int **pipes, t_exec *exec)
{
	command(token->redirects, pipes, 1, exec);
	if (token->type == CMD)
		command((t_cmd *)token->next, pipes, 0, exec);
	else if (token->type == OR)
		or_execute((t_node *)token->next, 1, pipes, exec);
	else if (token->type == AND)
		and_execute((t_node *)token->next, 1, pipes, exec);
	else if (token->type == PIPE)
		exec->exit_status = create_tree(0, (t_node *)token->next, *exec, pipes);
	else if (token->type == REDIR)
		redirect((t_redir *)token->next, pipes, 1, *exec);
	// ft_putstr_fd("CMD CLOSE: exit status:  ", 2);
	// ft_putnbr_fd(exec->exit_status, 2);
	// ft_putstr_fd("\n", 2);
}

void	test_exit_cmd(t_cmd *token, t_exec *exec)
{
	ft_putstr_fd(token->args[0], 2);
	ft_putstr_fd(token->args[1], 2);
	ft_putstr_fd("\n EXITSTATUS: ", 2);
	ft_putnbr_fd(exec->exit_status, 2);
	ft_putstr_fd("\n", 2);
}

void	test_exit_pipe(t_exec *exec)
{
	ft_putstr_fd("PIPE: exit status: ", 2);
	ft_putnbr_fd(exec->exit_status, 2);
	ft_putstr_fd("\n", 2);
}
int	redirect(t_redir *token, int **pipes, int status, t_exec exec)
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
		redirect_nodes(token, pipes, &exec);
		exit(exec.exit_status);
	}
	else
	{
		close_pipes(pipes);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exec.exit_status = WEXITSTATUS(status);
		// if (token->type == CMD)
		// 	test_exit_cmd((t_cmd *)token->next, &exec);
		// else if (token->type == PIPE)
		// 	test_exit_pipe(&exec);
		if (pipe_exit)
			exit_shell(&exec, NULL, exec.exit_status, 1);
	}
	return (exec.exit_status);
}

void	error_msg(char *cmd, char *file)
{
	char	*tmp;

	tmp = ft_strjoin(cmd, file);
	perror(tmp);
	free(tmp);
}
