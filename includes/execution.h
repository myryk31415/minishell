/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/03/25 13:09:49 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <sys/wait.h>

void	exec(char **cmd_arg, char **env);
void	close_pipes(int **pipes);
void	free_array(char **arr);
int		redirect(t_redir *token, int **pipes, int status, char **env);
int		output_handling(char **output, int *append);
int		input_handling(char **input, int *heredoc);
int		command_pipe(t_cmd *token, int **pipes, int redirect, char ***env);
char	*error_msg(char *cmd, char *file);
char	*get_env(char **environ, char *var);
char	*get_path(char *cmd, char **environ, char *var);
int		pwd(void);
int		echo(char **args);
int		cd(char	*arg, char ***env);
int		and_execute(t_node *token, int status, int **pipes, char ***env);
int		command_no_pipe(t_cmd *token, char ***env, int **pipes, int *redir);
int		or_execute(t_node *token, int status, char ***env);
void	run_tree(t_node *token, int **pipes, char ***env);
int		create_tree(int *pre_fd, t_node *token, int status, char **env);
void	execution(void *tree, t_node_type type, char ***env);
int		is_builtin(t_cmd *token, int **pipes, int *redir, char ***env);
void	command(t_cmd *token, int **pipes, int *redir);
int		export(char **arg, char ***env);
int		env(t_cmd *token, char **env);
void	display_env(char **env);
#endif
