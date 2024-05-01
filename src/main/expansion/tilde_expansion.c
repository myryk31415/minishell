/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:40:25 by padam             #+#    #+#             */
/*   Updated: 2024/04/25 15:55:47 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_tilde(char *c)
{
	if (!c[0] || c[0] != '~')
		return (0);
	if (!c[1] || c[1] == '/' || is_separator(c[1]))
		return (1);
	return (0);
}

char	*expand_tilde(char *cmd_arg, char **env)
{
	char	*home;
	char	*tmp;

	if (is_tilde(cmd_arg))
	{
		home = get_env(env, "HOME");
		tmp = ft_strjoin(home, cmd_arg + 1);
		free(home);
		free(cmd_arg);
		return (tmp);
	}
	return (cmd_arg);
}
