/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_env_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:57:00 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/30 16:40:51 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	check_flags(char **args)
{
	int	i;
	int	j;
	int	stop;

	stop = 0;
	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] && !stop)
			if (args[i][j++] != 'n')
				stop = 1;
		if (stop || !args[i][1])
			break ;
		i++;
	}
	return (i);
}

int	echo(char **args)
{
	int		i;
	bool	flag;

	if (!args || !*args)
		return (EXIT_FAILURE);
	i = check_flags(args);
	if (i >= 2)
		flag = true;
	else
		flag = false;
	if (args[i])
		if (ft_printf("%s", args[i++]) == -1)
			return (EXIT_FAILURE);
	while (args[i])
	{
		if (ft_printf(" %s", args[i]) == -1)
			return (EXIT_FAILURE);
		i++;
	}
	if (flag == false)
		ft_printf("\n");
	return (0);
}

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		return (free(cwd), perror("getcwd"), EXIT_FAILURE);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	env_cmd(t_cmd *token, char **env)
{
	(void)token;
	if (token->args[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return (1);
	}
	display_env(env, 0);
	return (0);
}

int	pwd_export(char *arg, char ***env, int pwd)
{
	char	*name;

	if (pwd == 1)
		name = ft_strdup("PWD");
	else
		name = ft_strdup("OLDPWD");
	if (arg)
	{
		if (check_if_assigned(name, env, arg) == -1)
		{
			*env = allocate_new_env(*env, arg);
			if (*env == NULL)
				return (free(name), EXIT_FAILURE);
		}
	}
	free(name);
	return (0);
}
