/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/25 00:01:40 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	g_signal;

char	**fill_env(void)
{
	int			i;
	extern char	**environ;
	char		**env;

	i = 0;
	while (environ[i])
		i++;
	env = malloc(sizeof(char *) * i + 1);
	if (!env)
		return (perror("Malloc"), NULL);
	i = -1;
	while (environ[++i])
	{
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
			perror("Malloc");
	}
	env[i] = NULL;
	return (env);
}

int	main(void)
{
	void		*token_tree;
	int			exit_status;
	char		**env;
	t_node_type	type;

	g_signal = 0;
	exit_status = 0;
	set_signal_action();
	env = fill_env();
	if (!env)
		return (-1);
	while (1)
	{
		token_tree = NULL;
		type = parser(&token_tree, exit_status),
		execution(token_tree, type, &env);

	}
	return (0);
}
