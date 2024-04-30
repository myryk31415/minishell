/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:47:36 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 16:36:52 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

extern int	g_signal;

char	*get_line(void)
{
	char	*line;
	char	*tmp;

	if (isatty(fileno(stdin)) && !DEBUG)
		line = readline(">");
	else
	{
		tmp = get_next_line(STDIN_FILENO);
		line = ft_strtrim(tmp, "\n");
	}
	return (line);
}

int	here_doc(char *limiter)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return(perror("minishell: Pipe:"), -1);
	while (1)
	{
		line = get_line();
		if (line && *line)
		{
			if (!ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
			{
				free(line);
				break ;
			}
			write(fd[1], line, ft_strlen(line));
			free(line);
		}
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	return (fd[0]);
}

int	handle_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->redirects[i])
	{
		if (cmd->redirect_type[i] >= 3)
		{
			cmd->redirects[i] = expander(cmd->redirects[i]);
			if (cmd->redirect_type[i] == 4)
				cmd->redirect_type[i] = here_doc(cmd->redirects[i]);
			else
				cmd->redirect_type[i] = -here_doc(cmd->redirects[i]);
			free(cmd->redirects[i]);
			cmd->redirects[i] = NULL;
			if (cmd->redirect_type[i] == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	handle_redir(t_redir *redir)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = redir->redirects;
	while (cmd->redirects[i])
	{
		if (cmd->redirect_type[i] >= 3)
		{
			cmd->redirects[i] = expander(cmd->redirects[i]);
			if (cmd->redirect_type[i] == 4)
				cmd->redirect_type[i] = here_doc(cmd->redirects[i]);
			else
				cmd->redirect_type[i] = -here_doc(cmd->redirects[i]);
			free(cmd->redirects[i]);
			cmd->redirects[i] = NULL;
			if (cmd->redirect_type[i] == -1)
				return (-1);
		}
		i++;
	}
	if (redir->next)
		climb_tree(redir->next, redir->type);
	return (0);
}

int	climb_tree(void *ptr, t_node_type type)
{
	t_node	*node;

	if (type == CMD)
		return (handle_cmd(ptr));
	else if (type == REDIR)
		return(handle_redir(ptr));
	else if (type == PIPE || type == AND || type == OR)
	{
		node = (t_node *)ptr;
		if (node->left)
			if (climb_tree(node->left, node->type_left) == -1)
				return (-1);
		if (node->right)
			return (climb_tree(node->right, node->type_right));
	}
	return (-1);
}
