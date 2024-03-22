/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:47:35 by antonweizma       #+#    #+#             */
/*   Updated: 2024/03/22 12:55:18 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] != '=')
		i++;
	name = malloc(i + 1);
	i = -1;
	while (arg[++i] != '=')
		name[i] = arg[i];
	name[i] = 0;
	return (name);
}

int	check_if_assigned(char *name, char **env, char *arg)
{
	// char	*tmp;
	int		j;

	j = 0;
	while (env[j])
	{
		if (!ft_strncmp(name, env[j], ft_strlen(name))
			&& (env[j])[ft_strlen(name)] == '=')
		{
			// tmp = env[j];
			env[j] = arg;
			// free(tmp);
			return (0);
		}
		j++;
	}
	return (-1);
}

int	allocate_new_env(char **env, char *arg)
{
	char	**new_env;
	int		env_size;
	char	**tmp;

	env_size = 0;
	while (env[env_size++])
		;
	new_env = malloc(sizeof(char *) * env_size + 2);
	if (!new_env)
		return (perror("Failed to allocate memory"), -1);
	env_size = -1;
	while (env[++env_size])
		new_env[env_size] = env[env_size];
	new_env[env_size++] = arg;
	new_env[env_size] = 0;
	tmp = env;
	env = new_env;
	free_array(tmp);
	return (0);
}

int	export(char **arg, char **env)
{
	char	*name;
	int		i;

	i = 0;
	arg++;
	name = get_name(arg[i]);
	while (arg[i])
	{
		if (check_if_assigned(name, env, arg[i]) == -1)
			if (allocate_new_env(env, arg[i]) == -1)
				return (free(name), -1);
		i++;
	}
	free(name);
	return (0);
}
