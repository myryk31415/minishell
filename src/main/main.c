/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 16:45:49 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	g_signal;

char	*get_env(char **environ, char *var)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (environ[i])
	{
		j = 0;
		while (environ[i][j] && environ[i][j] != '=')
			j++;
		tmp = ft_substr(environ[i], 0, j);
		if (!tmp)
			break ;
		if (!ft_strncmp(tmp, var, ft_strlen(var))
			&& !ft_strncmp(tmp, var, ft_strlen(tmp)))
		{
			free(tmp);
			return (ft_strdup(environ[i] + j + 1));
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

void	execution(void *tree, t_node_type type, t_exec *exec)
{
	signal(SIGINT, execution_handler);
	signal(SIGQUIT, execution_handler);
	if (type == CMD)
		command((t_cmd *)tree, NULL, 2, exec);
	else if (type == AND)
		and_execute((t_node *)tree, NULL, exec);
	else if (type == OR)
		or_execute((t_node *)tree, NULL, exec);
	else if (type == PIPE)
		exec->exit_status = create_tree(0, (t_node *)tree, exec, NULL);
	else if (type == REDIR)
		exec->exit_status = redirect((t_redir *)tree, NULL, 0, exec);
	if (exec->exit_status == 256)
		exec->exit_status = 1;
	return ;
}

char	**fill_env(void)
{
	int			i;
	extern char	**environ;
	char		**env;

	i = 0;
	while (environ[i])
		i++;
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (perror("Malloc"), NULL);
	i = -1;
	while (environ[++i])
	{
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
			perror("Malloc");
	}
	return (env);
}

t_exec	*fill_struct(void)
{
	t_exec		*exec;
	char		***env;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (perror("Malloc"), NULL);
	env = malloc(sizeof(char **));
	if (!env)
		return (perror("Malloc"), NULL);
	*env = fill_env();
	exec->env = env;
	exec->exit_status = EXIT_SUCCESS;
	exec->sub_process = 0;
	exec->tree = NULL;
	exec->type = 0;
	return (exec);
}

int	main(void)
{
	void		*token_tree;
	t_node_type	type;
	t_exec		*exec;

	g_signal = 0;
	exec = fill_struct();
	if (!exec)
		return (-1);
	while (1)
	{
		token_tree = NULL;
		ft_restore_terminal(0);
		ft_configure_terminal();
		type = parser(&token_tree, exec);
		ft_restore_terminal(1);
		exec->type = type;
		exec->tree = token_tree;
		if (!isatty(STDIN_FILENO) && type == ERROR)
			exit_shell(exec, NULL, exec->exit_status);
		execution(token_tree, type, exec);
		node_tree_delete(token_tree, type);
		exec->tree = NULL;
	}
	return (0);
}
