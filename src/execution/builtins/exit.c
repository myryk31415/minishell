/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:09:23 by padam             #+#    #+#             */
/*   Updated: 2024/03/28 14:27:56 by antonweizma      ###   ########.fr       */
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


void	exit_shell(t_exec *exec, char **arg, unsigned int status)
{
	unsigned int	exit_status;

	exit_status = status;
	if (arg && arg[1])
		exit_status = ft_u_atoi(arg[1]);

	free_env(exec->env);
	// rl_clear_history();
	node_tree_delete(exec->tree, exec->type);
	if (arg)
		free(exec);
	exit(exit_status);
}
