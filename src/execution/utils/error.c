/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:26:27 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 16:36:15 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	error_msg(char *cmd, char *file)
{
	char	*tmp;

	tmp = ft_strjoin(cmd, file);
	perror(tmp);
	free(tmp);
}

int	error_message_permission(char *cmd_path, int exit_status)
{
	DIR	*folder;
	int	file;

	folder = opendir(cmd_path);
	file = open(cmd_path, O_RDWR);
	if (file == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd_path);
		exit_status = 126;
		if (!folder && errno != EACCES)
			exit_status = 127;
	}
	else if (access(cmd_path, X_OK))
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd_path);
		exit_status = 126;
	}
	if (file)
		close(file);
	return (exit_status);
}

int	error_message(char *cmd_path)
{
	int	exit_status;

	exit_status = 0;
	if (!ft_strncmp(cmd_path, ".", 2))
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		exit_status = 127;
	}
	else if (ft_strchr(cmd_path, '/') == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putendl_fd(": command not found", 2);
		exit_status = 127;
	}
	else
		exit_status = error_message_permission(cmd_path, exit_status);
	return (exit_status);
}
