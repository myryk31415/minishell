/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 21:00:56 by antonweizma       #+#    #+#             */
/*   Updated: 2024/04/26 21:10:53 by antonweizma      ###   ########.fr       */
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

int	builtin(t_cmd *token, int **pipes, int *redir, t_exec *exec)
{
	int		old_stdin;
	int		old_stdout;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	exec->exit_status = is_builtin(token, pipes, redir, exec);
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
	if (exec->exit_status == -1)
	{
		exec->exit_status = 0;
		return (-1);
	}
	return (exec->exit_status);
}

int	new_waitpid(int status, int id)
{
	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
