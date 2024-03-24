/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/23 12:27:07 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

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

	exit_status = 0;
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
