/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 21:00:56 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/30 16:26:03 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**ft_sort_alpha(char **env)
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
			if (ft_strncmp(env[i], env[j], ft_strlen(env[i])) < 0 \
				&& ft_strncmp(env[i], env[j], ft_strlen(env[j])))
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

char	*get_export_env(char *str)
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
	int		i;
	char	*tmp;

	i = 0;
	if (export)
	{
		env = ft_sort_alpha(env);
		i = 0;
		while (env[i])
		{
			tmp = get_export_env(env[i]);
			ft_printf("declare -x %s\n", tmp);
			free(tmp);
			i++;
		}
	}
	else
		while (env[i])
			ft_printf("%s\n", env[i++]);
}

int	new_waitpid(int id)
{
	int	status;

	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
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
