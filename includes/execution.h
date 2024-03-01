/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/01 15:21:56 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>

void	exec(char **cmd_arg);
int		here_doc(char *limiter);
void	free_array(char **arr);
int		input(char **input, bool *heredoc);
int		output(char **output, bool *append);
void	command(t_cmd *token, int *fd, int *pre_fd);
char	*error_msg(char *cmd, char *file);
char	*get_env(char **envp, char *var);
int		pwd(void);
int		echo(char **args);
#endif
