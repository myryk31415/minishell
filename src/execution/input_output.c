/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:25:42 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/13 20:59:24 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	redirect(t_redir *token, int *fd, int *pre_fd, int status)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (!pid)
	{
		command_pipe(token->redirects, NULL, NULL, 1);
		if (token->type == CMD)
			command_pipe((t_cmd *)token->next, NULL, NULL, 0);
		else if (token->type == OR)
			or_execute((t_node *)token->next, fd, pre_fd, 0);
		else if (token->type == AND)
			and_execute((t_node *)token->next, fd, pre_fd, 0);
		else if (token->type == PIPE)
			create_tree(0, (t_node *)token->next, 0);
		else if (token->type == REDIR)
			command_pipe((t_cmd *)token->next, NULL, NULL, 1);
	}
	else
		waitpid(pid, &status, 0);
	return (status);
}

char	*error_msg(char *cmd, char *file)
{
	char	*tmp;

	tmp = ft_strjoin(cmd, file);
	return (tmp);
}
