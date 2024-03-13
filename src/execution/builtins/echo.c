/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:57:00 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/13 17:06:39 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	n_flag(char *flag)
{
	int	i;

	i = 0;
	if (flag)
	{
		if (flag[i] != '-')
			return (1);
		else
			i++;
		while (flag[i])
			if (flag[i++] != 'n')
				return (1);
	}
	return (2);
}

int	echo(char **args)
{
	int		i;
	bool	flag;

	if (!args | !*args)
		return (-1);
	i = n_flag(args[1]);
	if (i == 2)
		flag = true;
	else
		flag = false;
	if (args[i])
		if (ft_printf("%s", args[i++]) == 256)
			return (-1);
	while (args[i])
		if (ft_printf(" %s", args[i++]) == 256)
			return (-1);
	if (flag == false)
		ft_printf("\n");
	return (0);
}

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		return (free(cwd), perror("getcwd"), 256);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}
