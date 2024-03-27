/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:47:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/27 20:16:52 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	here_doc(char *limiter, int variable_expansion, int exit_status, char **env)
{
	char	*str;
	int		fd[2];

	if (pipe(fd) == -1)
		return(perror("minishell: Pipe:"), -1);
	while (1)
	{
		str = readline(">");
		if (str && *str)
		{
			if (variable_expansion)
				str = expand_variables(str, exit_status, env);
			if (!ft_strncmp(str, limiter, ft_strlen(limiter))
				&& !ft_strncmp(str, limiter, ft_strlen(str) - 1))
			{
				free(str);
				break ;
			}
			write(fd[1], str, ft_strlen(str));
			free(str);
		}
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	return (fd[0]);
}

int	handle_cmd(t_cmd *cmd, int exit_status, char **env)
{
	int	i;

	i = 0;
	while (cmd->redirect_in[i])
	{
		if (cmd->heredoc[i])
		{
			if (cmd->heredoc[i] == 2)
				cmd->heredoc[i] = here_doc(cmd->redirect_in[i], 0, exit_status, env);
			else
				cmd->heredoc[i] = here_doc(cmd->redirect_in[i], 1, exit_status, env);
			free(cmd->redirect_in[i]);
			cmd->redirect_in[i] = NULL;
			if (cmd->heredoc[i] == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	handle_redir(t_redir *redir, int exit_status, char **env)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = redir->redirects;
	while (cmd->redirect_in[i])
	{
		if (cmd->heredoc[i])
		{
			if (cmd->heredoc[i] == 2)
				cmd->heredoc[i] = here_doc(cmd->redirect_in[i], 0, exit_status, env);
			else
				cmd->heredoc[i] = here_doc(cmd->redirect_in[i], 1, exit_status, env);
			free(cmd->redirect_in[i]);
			cmd->redirect_in[i] = NULL;
			if (cmd->heredoc[i] == -1)
				return (-1);
		}
		i++;
	}
	if (redir->next)
		climb_tree(redir->next, redir->type, exit_status, env);
	return (0);
}

int	climb_tree(void *ptr, t_node_type type, int exit_status, char **env)
{
	t_node	*node;

	if (type == CMD)
		return (handle_cmd(ptr, exit_status, env));
	else if (type == REDIR)
		return(handle_redir(ptr, exit_status, env));
	else if (type == PIPE || type == AND || type == OR)
	{
		node = (t_node *)ptr;
		if (node->left)
			if (climb_tree(node->left, node->type_left, exit_status, env) == -1)
				return (-1);
		if (node->right)
			return (climb_tree(node->right, node->type_right, exit_status, env));
	}
	return (-1);
}
