/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:26:27 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 22:30:30 by padam            ###   ########.fr       */
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
	if (folder)
		closedir(folder);
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

void	*print_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (NULL);
}

int	print_option_export(char *str)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putchar_fd(str[0], 2);
	ft_putchar_fd(str[1], 2);
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("export: usage: export [name[=value] ...]\n", 1);
	return (2);
}
