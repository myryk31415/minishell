/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:57:38 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/26 21:01:03 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_pipes(int **pipes)
{
	if (pipes && pipes[1])
	{
		close(pipes[1][0]);
		close(pipes[1][1]);
	}
	if (pipes && pipes[0])
	{
		close(pipes[0][0]);
		close(pipes[0][1]);
	}
}

void	close_pipe(int *pipe)
{
	if (pipe)
	{
		close(pipe[0]);
		close(pipe[1]);
	}
}

void	oldpwd_save(char ***env, char *path_to_dir, char *arg)
{
	char		*var;

	var = getcwd(NULL, PATH_MAX);
	pwd_export(ft_strjoin("OLDPWD=", var), env, 0);
	free(var);
	chdir(path_to_dir);
	pwd_export(ft_strjoin("PWD=", path_to_dir), env, 1);
	if (arg && arg[0] == '-')
		pwd();
	free (path_to_dir);
}

void	close_in_out_file_nofork(int input, int output, int *redir, int **pipes)
{
	if (pipes && pipes[1])
	{
		close(pipes[1][0]);
		close(pipes[1][1]);
	}
	if (pipes && pipes[0])
	{
		close(pipes[0][0]);
		close(pipes[0][1]);
	}
	if (output != 1 && (!redir || (redir && !redir[1])))
		close(output);
	if (input)
		close(input);
}

void	close_in_out_files_fork(int **pipes)
{
	if (pipes && pipes[1])
	{
		close(pipes[1][1]);
	}
	if (pipes && pipes[0])
	{
		close(pipes[0][0]);
	}
}
