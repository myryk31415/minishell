/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:16:36 by padam             #+#    #+#             */
/*   Updated: 2024/03/26 16:24:11 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

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
		type = parser(&token_tree, exit_status);
		execution(token_tree, type, &env);
	}
	return (0);
}
