/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:09:23 by padam             #+#    #+#             */
/*   Updated: 2024/04/30 17:18:46 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
	{
		return (1);
	}
	return (0);
}

unsigned int	ft_u_atoi(const char *str)
{
	int		negative;
	long	number;

	negative = 1;
	number = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			negative *= -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		number = number * 10 + *str - 48;
		str++;
	}
	return ((unsigned int)(number * negative));
}

int	exit_shell(t_exec *exec, char **arg, int status)
{
	int	exit_status;

	exit_status = status;
	if (status == -1)
		exit_status = exec->exit_status;
	if (arg && arg[1] && !ft_isnumber(arg[1]))
	{
		exit_status = 2;
		ft_putstr_fd("minishell: exit: \
numeric argument required\n", 2);
	}
	else if (arg && arg[1] && arg[2])
		return (ft_putstr_fd("minishell: exit: \
too many arguments\n", 2), 1);
	else if (arg && arg[1] && ft_isnumber(arg[1]))
	{
		exit_status = ft_u_atoi(arg[1]);
	}
	free_env(exec->env);
	node_tree_delete(exec->tree, exec->type);
	if (!exec->sub_process)
		free(exec);
	rl_clear_history();
	exit(exit_status);
}
