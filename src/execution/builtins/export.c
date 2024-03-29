/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:47:35 by antonweizma       #+#    #+#             */
/*   Updated: 2024/03/28 16:02:48 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg && arg[i] != '=')
		i++;
	name = malloc(i + 1);
	i = -1;
	while (arg[++i] != '=')
		name[i] = arg[i];
	name[i] = '\0';
	return (name);
}

int	check_if_assigned(char *name, char ***env, char *arg)
{
	int		j;
	char	*tmp;

	j = 0;
	while (env[0][j])
	{
		if (!ft_strncmp(name, env[0][j], ft_strlen(name))
			&& (env[0][j])[ft_strlen(name)] == '=')
		{
			tmp = env[0][j];
			env[0][j] = arg;
			free(tmp);
			return (0);
		}
		j++;
	}
	return (-1);
}

char	**allocate_new_env(char **env, char *arg)
{
	char	**new_env;
	int		env_size;
	char	**tmp;

	env_size = 0;
	while (env[env_size++])
		;
	new_env = malloc(sizeof(char *) * env_size + 1);
	if (!new_env)
		return (perror("Failed to allocate memory"), NULL);
	env_size = -1;
	while (env[++env_size])
	{
		new_env[env_size] = ft_strdup(env[env_size]);
		if (!new_env[env_size])
			perror("Malloc");
	}
	new_env[env_size++] = ft_strdup(arg);
	new_env[env_size] = 0;
	tmp = env;
	env = new_env;
	free_str_array(tmp);
	return (env);
}

int	pwd_export(char *arg, char ***env)
{
	char	*name;

	name = ft_strdup("OLDPWD");
	if (arg)
	{
		if (check_if_assigned(name, env, arg) == -1)
		{
			*env = allocate_new_env(*env, arg);
			if (*env == NULL)
				return (free(name), -1);
		}
	}
	free(name);
	return (0);
}


int	export(char **arg, char ***env)
{
	char	*name;
	int		i;

	i = 0;
	if (ft_strncmp("OLDPWD=", arg[i], 7) && arg[1])
		arg++;
	if (!ft_strncmp("export", arg[i], 6) && !arg[1])
		return (display_env(*env), 0);
	name = get_name(arg[i]);
	while (arg[i])
	{
		if (check_if_assigned(name, env, arg[i]) == -1)
		{
			*env = allocate_new_env(*env, arg[i]);
			if (*env == NULL)
				return (free(name), -1);
		}
		i++;
	}
	free(name);
	return (0);
}
