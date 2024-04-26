/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:57:38 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/26 13:22:29 by antonweizma      ###   ########.fr       */
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

void	close_in_out_files_fork(int input, int output, int *redir, int **pipes)
{
	if (pipes && pipes[1])
	{
		close(pipes[1][0]);
		close(pipes[1][1]);
	}
	if (pipes && pipes[0])
	{
		close(pipes[0][0]);
		// close(pipes[0][1]);
	}
	if (output != 1 && (!redir || (redir && !redir[1])))
		close(output);
	if (input)
		close(input);
}

char **ft_sort_alpha(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[j])
		{
			if (ft_strncmp(env[i], env[j], ft_strlen(env[i])) < 0 && ft_strncmp(env[i], env[j], ft_strlen(env[j])))
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (env);
}

char *get_export_env(char *str)
{
	int		i;
	char	*tmp;
	char	*out;
	char	*tmp2;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
	tmp = ft_substr(str, 0, i);
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, "\"");
	free(tmp);
	tmp = ft_substr(str, i, ft_strlen(str));
	if (!tmp)
		return (NULL);
	tmp2 = ft_strjoin(out, tmp);
	free(tmp);
	free(out);
	out = ft_strjoin(tmp2, "\"");
	free(tmp2);
	return (out);
}

void	display_env(char **env, int export)
{
	int	i;

	i = 0;
	if (export)
	{
		env = ft_sort_alpha(env);
		i = 0;
		while (env[i])
			ft_printf("declare -x %s\n", get_export_env(env[i++]));
	}
	else
		while (env[i])
			ft_printf("%s\n", env[i++]);
}
