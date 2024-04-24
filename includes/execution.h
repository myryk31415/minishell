/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/24 13:21:42 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <sys/wait.h>
# include <errno.h>

void	execute(char **cmd_arg, t_exec *exec);
void	close_pipes(int **pipes);
void	free_str_array(char **arr);
int		redirect(t_redir *token, int **pipes, int status, t_exec exec);
int		handle_both(int *redir, char **redirects, int *redirect_type, t_exec *exec);
// int		output_handling(char **output, int *append, t_exec *exec);
// int		input_handling(char **input, int *heredoc, t_exec *exec);
void	command(t_cmd *token, int **pipes, int redirect, t_exec *exec);
void	error_msg(char *cmd, char *file);
char	*get_path(char *cmd, char **environ, char *var);
int		pwd(void);
int		echo(char **args);
int		cd(char	**arg, char ***env);
void	exit_shell(t_exec *exec, char **arg, unsigned int status);
void	oldpwd_save(char ***env, char *path_to_dir, char *arg);
void	and_execute(t_node *token, int status, int **pipes, t_exec *exec);
void	command_fork(t_cmd *token, t_exec *exec, int **pipes, int *redir);
void	or_execute(t_node *token, int status, int **pipes, t_exec *exec);
void	run_tree(t_node *token, int **pipes, t_exec *exec, int id);
int		create_tree(int *pre_fd, t_node *token, t_exec exec);
void	execution(void *tree, t_node_type type, t_exec *exec);
int		is_builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec);
int		check_valid(char **args, char *str);
int		in_and_out_handling(t_cmd *token, int **pipes, int *redir, t_exec *exec);
int		export(char **arg, char ***env, int i);
void	close_pipe(int *pipe);
int		env_cmd(t_cmd *token, char **env);
void	display_env(char **env, int export);
int		unset(char **args, char ***env);
int		pwd_export(char *arg, char ***env, int i);
void	close_in_and_out_files(int input, int output, int *redir, int **pipes);
void	free_env(char ***env);
#endif
