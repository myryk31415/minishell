/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:47:36 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 12:05:08 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

extern int	g_signal;

static void	ft_heredoc_handler(int sig)
{
	ft_printf("\n");
	rl_replace_line(NULL, 0);
	(void)sig;
}

int	here_doc(char *limiter)
{
	char	*str;
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return(perror("minishell: Pipe:"), -1);
	while (1)
	{
		if (isatty(fileno(stdin)) && !DEBUG) //debug
		{
			signal(SIGINT, ft_heredoc_handler);
			str = readline(">");
			signal(SIGINT, parser_handler);
			ft_putstr_fd("AFSAFAFAF\n", 1);
		}
		else
		{
			line = get_next_line(STDIN_FILENO);
			str = ft_strtrim(line, "\n");
			free(line);
		}
		if (str && *str)
		{
			if (!ft_strncmp(str, limiter, ft_strlen(limiter) + 1))
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

char *extract_test(char *old_string, char *string)
{
	char *output;

	output = ft_strjoin(old_string, string);
	free(old_string);
	free(string);
	return (output);
}

char *expander_test(char *arg)
{
	char *output;
	char *tmp;
	int i;

	tmp = arg;
	output = NULL;
	while (*arg)
	{
		i = 0;
		if (*arg == '\'')
		{
			arg++;
			while (arg[i] != '\'')
				i++;
			output = extract_test(output, ft_substr(arg, 0, i));
			i++;
		}
		else if (*arg == '"')
		{
			arg++;
			while (arg[i] != '"')
				i++;
			output = extract_test(output, ft_substr(arg, 0, i));
			i++;
		}
		else
		{
			while (arg[i] && arg[i] != '\'' && arg[i] != '"')
				i++;
			output = extract_test(output, ft_substr(arg, 0, i));
		}
		arg += i;
	}
	free(tmp);
	return (output);
}

int	handle_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->redirects[i])
	{
		if (cmd->redirect_type[i] >= 3)
		{
			cmd->redirects[i] = expander_test(cmd->redirects[i]);
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
			cmd->redirects[i] = expander_test(cmd->redirects[i]);
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
