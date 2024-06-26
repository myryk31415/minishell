/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:47:35 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 16:40:47 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (print_error(arg));
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && !ft_strchr("_", arg[i]) \
			&& !(arg[i] == '+' && arg[i + 1] == '='))
			return (print_error(arg));
		i++;
	}
	if (i == 0)
		return (print_error(arg));
	if (!arg[i])
		i = 0;
	name = ft_calloc(i + 1, 1);
	while (i-- > 0)
		name[i] = arg[i];
	return (name);
}

int	check_append(char ***env, char *arg, char *name, int j)
{
	char	*tmp;
	char	*tmp2;

	while (env[0][j])
	{
		if (!ft_strncmp(name, env[0][j], ft_strlen(name) - 1)
			&& (env[0][j])[ft_strlen(name) - 1] == '=')
		{
			tmp = env[0][j];
			tmp2 = ft_substr(arg, ft_strlen(name) + 1, \
				ft_strlen(arg) - ft_strlen(name) - 1);
			env[0][j] = ft_strjoin(tmp, tmp2);
			free(tmp);
			free(tmp2);
			return (0);
		}
		j++;
	}
	return (1);
}

int	check_if_assigned(char *name, char ***env, char *arg)
{
	int		j;
	char	*tmp;

	j = 0;
	if (ft_strchr(name, '+'))
		return (check_append(env, arg, name, j));
	while (env[0][j])
	{
		if (!ft_strncmp(name, env[0][j], ft_strlen(name))
			&& (env[0][j])[ft_strlen(name)] == '=')
		{
			tmp = env[0][j];
			env[0][j] = ft_strdup(arg);
			return (free(tmp), 0);
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
	while (env[env_size])
		env_size++;
	new_env = ft_calloc(env_size + 2, sizeof(char *));
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

int	export(char **arg, char ***env, int i)
{
	char	*name;

	if (ft_strncmp("OLDPWD=", arg[0], 7) && arg[1])
		arg++;
	else if (!ft_strncmp("export", arg[0], 6) && !arg[1])
		return (display_env(*env, 1), 0);
	if (arg[0][0] == '-')
		return (print_option_export(arg[0]));
	while (arg[i])
	{
		name = get_name(arg[i]);
		if (!name)
			return (EXIT_FAILURE);
		if (!*name)
			return (free(name), 0);
		if (check_if_assigned(name, env, arg[i]) == -1)
		{
			*env = allocate_new_env(*env, arg[i]);
			if (*env == NULL)
				return (free(name), -1);
		}
		i++;
		free (name);
	}
	return (0);
}
