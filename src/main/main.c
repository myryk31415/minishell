/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/04/25 20:24:13 by padam            ###   ########.fr       */
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
	return (exec);
}

int	main(void)
{
	void		*token_tree;
	t_node_type	type;
	t_exec		*exec;

	g_signal = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	// set_signal_action();
	exec = fill_struct();
	if (!exec)
		return (-1);
	while (1)
	{
		token_tree = NULL;
		type = parser(&token_tree, exec);
		exec->type = type;
		exec->tree = token_tree;
		if (!isatty(STDIN_FILENO) && type == ERROR)
			exit_shell(exec, NULL, exec->exit_status);
		// system("leaks minishell");
		execution(token_tree, type, exec);
		// ft_printf("%i\n", exec->exit_status);
		node_tree_delete(token_tree, type);
		// system("leaks minishell");
	}
	free_env(exec->env);
	return (0);
}
