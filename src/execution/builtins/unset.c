/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:18:20 by antonweizma       #+#    #+#             */
/*   Updated: 2024/03/28 16:03:25 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**allocate_smaller_env(char **env, int i)
{
	char	**new_env;
	int		env_size;
	int		j;

	j = 0;
	env_size = 0;
	while (env[env_size])
		env_size++;
	new_env = malloc(sizeof(char *) * env_size);
	if (!new_env)
		return (perror("Failed to allocate memory"), NULL);
	env_size = -1;
	while (env[++env_size + j])
	{
		new_env[env_size] = ft_strdup(env[env_size + j]);
		if (!new_env[env_size])
			perror("Malloc");
		if (env_size + 1 == i)
			j = 1;
	}
	new_env[env_size] = NULL;
	free_str_array(env);
	return (new_env);
}

int	unset(char **args, char ***env)
{
	int	i;

	if (!args | !*args)
		return (EXIT_FAILURE);
	args++;
	while (*args)
	{
		i = -1;
		while (env[0][++i])
		{
			if (!ft_strncmp(*args, env[0][i], ft_strlen(*args))
				&& (env[0][i])[ft_strlen(*args)] == '=')
			{
				env[0] = allocate_smaller_env(*env, i);
				break ;
			}
		}
		args++;
	}
	return (0);
}
