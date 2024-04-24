/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:57:00 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/23 23:47:15 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	n_flag(char **flag)
{
	int	i;
	int	l;

	l = 1;
	while (flag && flag[l])
	{
		i = 0;
		if (flag[l][i] != '-')
			return (l);
		else
			i++;
		while (flag[l][i])
			if (flag[l][i++] != 'n')
				return (l);
		l++;
	}
	return (l);
}

int	echo(char **args)
{
	int		i;
	bool	flag;

	if (!args | !*args)
		return (EXIT_FAILURE);
	if (!args[1])
		return (ft_printf("\n"), 0);
	i = n_flag(args);
	if (i >= 2)
		flag = true;
	else
		flag = false;
	if (args[i])
		if (ft_printf("%s", args[i++]) == -1)
			return (EXIT_FAILURE);
	while (args[i])
		if (ft_printf(" %s", args[i++]) == -1)
			return (EXIT_FAILURE);
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
	// char	**tmp_env;
	// char	**tmp_var;
	(void)token;
	// tmp_env = env;
	// tmp_var = malloc(sizeof(char *) + 1);
	// if (!tmp_var)
	// 	return (-1);
	// if (!token->args)
	display_env(env);
	// else
	// {
	// 	tmp_var[0] = token->args[1];
	// 	tmp_var[1] = NULL;
	// 	export(tmp_var, &tmp_env);
	// 	command(token, NULL, 3, &tmp_env);
	// }
	// free(tmp_var);
	return (0);
}
