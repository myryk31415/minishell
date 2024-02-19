/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/02/19 17:47:32 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>

void	exec(char *cmd);
void	here_doc(char *limiter, int *fd);
void	free_array(char **arr);
void	input(t_redirect_in *token, int *fd);
void	output(t_redirect_out *token, int *fd);
void	cmd(char *cmd, int *fd, int *pre_fd);

#endif
