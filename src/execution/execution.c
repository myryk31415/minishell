/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:09 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/19 13:05:13 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// int	execution(t_node *token, t_redirect_in *input, t_redirect_out *output)
// {
// 	if (token->type == PIPE)
// 		create_tree(token->left, token->right);
// 	if (token->type == CMD)
// 		exec(token->command);
// }

void	cmd(char *cmd, int *fd, int *pre_fd)
{
	dup2(pre_fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(pre_fd[0]);
	close(pre_fd[1]);
	exec(cmd);
}

void	input_files(char *file_str, int *fd)
{
	char	*str;
	int		file;

	file = open(file_str, O_RDONLY, 0666);
	if (file == -1)
	{
		perror("Infile");
	}
	str = NULL;
	while (1)
	{
		str = get_next_line(file);
		if (str && *str)
		{
			write(fd[1], str, ft_strlen(str));
			free(str);
		}
		else
			break ;
	}
	close(file);
}

void	input(t_redirect_in *token, int *fd, int i)
{
	int			j;

	j = -1;
	if (i == 1)
	{
		while (++j < 4)
		{
			// ft_printf("%i", j);
			// ft_printf("%s\n", token->string[j]);
			if (token->heredoc[j] == false)
				input_files(token->string[j], fd);
			else
				here_doc(token->string[j], fd);
		}
		close(fd[1]);
	}
}

void	create_tree(int *pre_fd, t_node *token, int commands)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		perror("Pipe");
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pid == 0 && token->type_right == PIPE)
		create_tree(fd, (t_node *)token->right, commands);
	else if (pid != 0 && commands == 1)
		input((t_redirect_in *)token->left, fd, (token->type_left == REDIR_IN));
	// else if (pid != 0 && token->type_right == REDIR_OUT)
	// 	output((t_redirect_out *)token->right, fd);
	else
		cmd((char *)token->left, fd, pre_fd);
}
