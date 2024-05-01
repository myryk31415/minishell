/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:43:29 by aweizman          #+#    #+#             */
/*   Updated: 2024/04/30 18:45:30 by antonweizma      ###   ########.fr       */
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

// Builtins
int		pwd(void);
int		echo(char **args);
int		cd(char	**arg, char ***env);
int		env_cmd(t_cmd *token, char **env);
int		export(char **arg, char ***env, int i);
int		unset(char **args, char ***env);
int		is_builtin_no_fork(t_cmd *token, int **pipes, int *redir, t_exec *exec);
int		is_builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec);
int		builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec);

// Pipe Node
int		create_tree(int *pre_fd, t_node *token, t_exec *tmp, int **redir_pipes);
void	run_tree(t_node *token, int **pipes, t_exec *exec, int **redir_pipes);
void	close_pipe(int *pipe);
void	close_pipes(int **pipes);

// Input and Output
int		in_and_out_hdl_fork(t_cmd *token, int **pipes, \
		int *redir, t_exec *exec);
int		in_and_out_hdl_no_fork(t_cmd *token, int **pipes, \
		int *redir, t_exec *exec);
void	close_in_out_file_nofork(int input, int output, \
		int *redir, int **pipes);
void	close_in_out_files_fork(int **pipes);
int		handle_both(int *redir, t_cmd *token, t_exec *exec);

// Redir Node
int		redirect(t_redir *token, int **pipes, int status, t_exec *exec);

// And Or Nodes
void	and_execute(t_node *token, int **pipes, t_exec *exec);
void	or_execute(t_node *token, int **pipes, t_exec *exec);

// Command Nodes
void	command(t_cmd *token, int **pipes, int redirect, t_exec *exec);
int		new_waitpid(int id);

// Utils Error
void	error_msg(char *cmd, char *file);
int		error_message(char *cmd_path);
void	*print_error(char *arg);

// Utils Exexcute
void	execute(char **cmd_arg, t_exec *exec, int no_exec);
char	*get_path(char *cmd, char **environ, char *var);

// Utils Builtins
void	oldpwd_save(char ***env, char *path_to_dir, char *arg);
int		check_valid(char **args, char *str);
void	display_env(char **env, int export);
int		pwd_export(char *arg, char ***env, int i);
int		check_if_assigned(char *name, char ***env, char *arg);
char	**allocate_new_env(char **env, char *arg);
int		print_option_export(char *str);
int		pwd_export(char *arg, char ***env, int pwd);
#endif
