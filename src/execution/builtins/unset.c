/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:18:20 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/24 16:16:33 by antonweizma      ###   ########.fr       */
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

int	check_valid(char **args, char *str)
{
	int		i;
	int		j;
	char	*tmp;
	char	*out;

	i = 0;
	while (args[i])
	{
		j = -1;
		while (args[i][++j])
		{
			if (args[i][j] && !ft_isalnum(args[i][j]) && args[i][j] != '_')
			{
				tmp = ft_strjoin("`", args[i]);
				out = ft_strjoin(tmp, "'");
				ft_putstr_fd(str, 2);
				ft_putstr_fd(out, 2);
				ft_putendl_fd(": not a valid identifier", 2);
				return (free(tmp), 1);
			}
		}
		i++;
	}
	return (0);
}

int	print_option(char *str)
{
	ft_putstr_fd("minishell: unset: ", 1);
	ft_putchar_fd(str[0], 1);
	ft_putchar_fd(str[1], 1);
	ft_putstr_fd(": invalid option\n", 1);
	ft_putstr_fd("unset: usage: unset [name ...]\n", 1);
	return (2);
}

int	unset(char **args, char ***env)
{
	int		i;

	if (!args || !*args)
		return (EXIT_FAILURE);
	if (args[1] && args[1][0] == '-')
		return (print_option(args[1]));
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
