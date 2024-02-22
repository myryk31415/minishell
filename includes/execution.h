/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/22 14:56:59 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>

void	exec(char **cmd_arg);
int		here_doc(char *limiter);
void	free_array(char **arr);
int		input(t_redirect_in *token);
int		output(t_redirect_out *token);
void	command(t_cmd *token, int *fd, int *pre_fd);
char	*error_msg(char *cmd, char *file);

#endif
