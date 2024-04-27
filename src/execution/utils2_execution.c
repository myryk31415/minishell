/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:57:38 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/27 21:29:29 by padam            ###   ########.fr       */
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
	char		*tmp;

	var = getcwd(NULL, PATH_MAX);
	tmp = ft_strjoin("OLDPWD=", var);
	pwd_export(tmp, env, 0);
	free(tmp);
	free(var);
	chdir(path_to_dir);
	tmp = ft_strjoin("PWD=", path_to_dir);
	pwd_export(tmp, env, 1);
	free(tmp);
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
	free(pipes);
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
