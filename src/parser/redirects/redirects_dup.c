/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:10:31 by padam             #+#    #+#             */
/*   Updated: 2024/03/04 20:57:24 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	cmd_free(t_cmd *cmd)
{
	free_str_array(cmd->args);
	free_str_array(cmd->redirect_in);
	free_str_array(cmd->redirect_out);
	free(cmd->heredoc);
	free(cmd->append);
	free(cmd);
}

bool	*dup_bool_array(bool *arr, int len)
{
	bool	*new_arr;

	if (!len)
		return (NULL);
	new_arr = ft_calloc(len, sizeof(bool));
	if (!new_arr)
		return (NULL);
	while (len--)
		new_arr[len] = arr[len];
	return (new_arr);
}

int	get_array_len(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**dup_str_array(char **arr, int len)
{
	char	**new_arr;

	if (!len)
		return (NULL);
	new_arr = ft_calloc(len + 1, sizeof(char *));
	if (!new_arr)
		return (NULL);
	new_arr[len] = NULL;
	while (len--)
	{
		new_arr[len] = ft_strdup(arr[len]);
		if (!new_arr[len])
		{
			free_str_array(new_arr);
			return (NULL);
		}
	}
	return (new_arr);
}

t_cmd	*redirects_dup(t_cmd	*redirects)
{
	t_cmd	*new_redirects;
	int		i;

	i = 0;
	new_redirects = ft_calloc(1, sizeof(t_cmd));
	if (!new_redirects)
		return (NULL);
	new_redirects->args = NULL;
	i = get_array_len(redirects->redirect_in);
	new_redirects->redirect_in = dup_str_array(redirects->redirect_in, i);
	new_redirects->heredoc = dup_bool_array(redirects->heredoc, i);
	i = get_array_len(redirects->redirect_out);
	new_redirects->redirect_out = dup_str_array(redirects->redirect_out, i);
	new_redirects->append = dup_bool_array(redirects->append, i);
	return (new_redirects);
}
