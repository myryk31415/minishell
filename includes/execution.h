/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/13 20:58:10 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <sys/wait.h>

void	exec(char **cmd_arg);
int		here_doc(char *limiter);
void	free_array(char **arr);
int		redirect(t_redir *token, int *fd, int *pre_fd, int status);
int		command_pipe(t_cmd *token, int *fd, int *pre_fd, int redirect);
char	*error_msg(char *cmd, char *file);
char	*get_env(char **environ, char *var);
char	*get_path(char *cmd, char **environ, char *var);
int		pwd(void);
int		echo(char **args);
int		cd(char	*arg);
int		and_execute(t_node *token, int *fd, int *pre_fd, int status);
int		or_execute(t_node *token, int *fd, int *pre_fd, int status);
void	run_tree(int *pre_fd, t_node *token, int fd[2]);
int		create_tree(int *pre_fd, t_node *token, int status);
void	execution(void *tree, t_node_type type);
int		is_builtin(t_cmd *token, int *fd, int *pre_fd, int *redir);
void	command(t_cmd *token, int *fd, int *pre_fd, int *redir);
#endif
